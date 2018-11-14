/******************************************************************************
 *                       ____    _    _____                                   *
 *                      / ___|  / \  |  ___|    C++                           *
 *                     | |     / _ \ | |_       Actor                         *
 *                     | |___ / ___ \|  _|      Framework                     *
 *                      \____/_/   \_|_|                                      *
 *                                                                            *
 * Copyright 2011-2018 Dominik Charousset                                     *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License or  *
 * (at your option) under the terms and conditions of the Boost Software      *
 * License 1.0. See accompanying files LICENSE and LICENSE_ALTERNATIVE.       *
 *                                                                            *
 * If you did not receive a copy of the license files, see                    *
 * http://opensource.org/licenses/BSD-3-Clause and                            *
 * http://www.boost.org/LICENSE_1_0.txt.                                      *
 ******************************************************************************/

#include "caf/stream_manager.hpp"

#include "caf/actor_addr.hpp"
#include "caf/actor_cast.hpp"
#include "caf/actor_control_block.hpp"
#include "caf/error.hpp"
#include "caf/expected.hpp"
#include "caf/inbound_path.hpp"
#include "caf/scheduled_actor.hpp"
#include "caf/logger.hpp"
#include "caf/message.hpp"
#include "caf/outbound_path.hpp"
#include "caf/response_promise.hpp"
#include "caf/sec.hpp"

namespace caf {

stream_manager::stream_manager(scheduled_actor* selfptr, stream_priority prio)
    : self_(selfptr),
      pending_handshakes_(0),
      priority_(prio),
      continuous_(false) {
  // nop
}

stream_manager::~stream_manager() {
  // nop
}

void stream_manager::handle(inbound_path*, downstream_msg::batch&) {
  CAF_LOG_WARNING("unimplemented base handler for batches called");
}

void stream_manager::handle(inbound_path*, downstream_msg::close&) {
  // nop
}

void stream_manager::handle(inbound_path*, downstream_msg::forced_close& x) {
  abort(std::move(x.reason));
}

bool stream_manager::handle(stream_slots slots, upstream_msg::ack_open& x) {
  CAF_LOG_TRACE(CAF_ARG(slots) << CAF_ARG(x));
  CAF_ASSERT(x.desired_batch_size > 0);
  auto ptr = out().path(slots.receiver);
  if (ptr == nullptr)
    return false;
  if (!ptr->pending()) {
    CAF_LOG_ERROR("received repeated ack_open");
    return false;
  }
  if (ptr->hdl != x.rebind_from) {
    CAF_LOG_ERROR("received ack_open with invalid rebind_from");
    return false;
  }
  if (x.rebind_from != x.rebind_to) {
    ptr->hdl = x.rebind_to;
  }
  ptr->slots.receiver = slots.sender;
  ptr->open_credit = x.initial_demand;
  ptr->desired_batch_size = x.desired_batch_size;
  --pending_handshakes_;
  push();
  return true;
}

void stream_manager::handle(stream_slots slots, upstream_msg::ack_batch& x) {
  CAF_LOG_TRACE(CAF_ARG(slots) << CAF_ARG(x));
  CAF_ASSERT(x.desired_batch_size > 0);
  auto path = out().path(slots.receiver);
  if (path != nullptr) {
    path->open_credit += x.new_capacity;
    path->desired_batch_size = x.desired_batch_size;
    path->next_ack_id = x.acknowledged_id + 1;
    push();
  }
}

void stream_manager::handle(stream_slots slots, upstream_msg::drop&) {
  error tmp;
  out().remove_path(slots.receiver, std::move(tmp), true);
}

void stream_manager::handle(stream_slots slots, upstream_msg::forced_drop& x) {
  if (out().remove_path(slots.receiver, x.reason, true))
    abort(std::move(x.reason));
}

void stream_manager::stop() {
  CAF_LOG_TRACE("");
  out().close();
  error tmp;
  finalize(tmp);
  self_->erase_inbound_paths_later(this);
}

void stream_manager::abort(error reason) {
  CAF_LOG_TRACE(CAF_ARG(reason));
  out().abort(reason);
  finalize(reason);
  self_->erase_inbound_paths_later(this, std::move(reason));
}

void stream_manager::push() {
  CAF_LOG_TRACE("");
  do {
    out().emit_batches();
  } while (generate_messages());
}

bool stream_manager::congested() const noexcept {
  return false;
}

void stream_manager::deliver_handshake(response_promise& rp, stream_slot slot,
                                       message handshake) {
  CAF_LOG_TRACE(CAF_ARG(rp) << CAF_ARG(slot) << CAF_ARG(handshake));
  CAF_ASSERT(rp.pending());
  CAF_ASSERT(slot != invalid_stream_slot);
  ++pending_handshakes_;
  auto next = rp.next();
  rp.deliver(open_stream_msg{slot, std::move(handshake), self_->ctrl(), next,
                             priority_});
}

bool stream_manager::generate_messages() {
  return false;
}

void stream_manager::cycle_timeout(size_t) {
  // TODO: make pure virtual
}

void stream_manager::register_input_path(inbound_path* ptr) {
  CAF_ASSERT(ptr != nullptr);
  CAF_LOG_TRACE(CAF_ARG2("path", *ptr));
  inbound_paths_.emplace_back(ptr);
}

void stream_manager::deregister_input_path(inbound_path* ptr) noexcept {
  CAF_ASSERT(ptr != nullptr);
  CAF_LOG_TRACE(CAF_ARG2("path", *ptr));
  CAF_ASSERT(inbound_paths_.size() > 0);
  using std::swap;
  if (ptr != inbound_paths_.back()) {
    auto i = std::find(inbound_paths_.begin(), inbound_paths_.end(), ptr);
    CAF_ASSERT(i != inbound_paths_.end());
    swap(*i, inbound_paths_.back());
  }
  inbound_paths_.pop_back();
  CAF_LOG_DEBUG(inbound_paths_.size() << "paths remaining");
}

void stream_manager::remove_input_path(stream_slot slot, error reason,
                                       bool silent) {
  if (silent)
    self_->erase_inbound_path_later(slot);
  else
    self_->erase_inbound_path_later(slot, std::move(reason));
}

inbound_path* stream_manager::get_inbound_path(stream_slot x) const noexcept {
  auto pred = [=](inbound_path* ptr) {
    return ptr->slots.receiver == x;
  };
  auto e = inbound_paths_.end();
  auto i = std::find_if(inbound_paths_.begin(), e, pred);
  return i != e ? *i : nullptr;
}


bool stream_manager::inbound_paths_up_to_date() const noexcept {
  auto up_to_date = [](inbound_path* x) { return x->up_to_date(); };
  return std::all_of(inbound_paths_.begin(), inbound_paths_.end(), up_to_date);
}

stream_slot stream_manager::add_unchecked_outbound_path_impl(response_promise& rp,
                                                          message handshake) {
  CAF_LOG_TRACE(CAF_ARG(rp) << CAF_ARG(handshake));
  CAF_ASSERT(out().terminal() == false);
  if (!rp.pending()) {
    CAF_LOG_WARNING("add_outbound_path called with next == nullptr");
    rp.deliver(sec::no_downstream_stages_defined);
    return invalid_stream_slot;
  }
  auto slot = self_->assign_next_pending_slot_to(this);
  auto path = out().add_path(slot, rp.next());
  CAF_IGNORE_UNUSED(path);
  CAF_ASSERT(path != nullptr);
  // Build pipeline by forwarding handshake along the path.
  deliver_handshake(rp, slot, std::move(handshake));
  generate_messages();
  return slot;
}

stream_slot stream_manager::add_unchecked_outbound_path_impl(strong_actor_ptr next,
                                                          message handshake) {
  CAF_LOG_TRACE(CAF_ARG(next) << CAF_ARG(handshake));
  response_promise rp{self_->ctrl(), nullptr, {next}, make_message_id()};
  return add_unchecked_outbound_path_impl(rp, std::move(handshake));
}

stream_slot stream_manager::add_unchecked_outbound_path_impl(message handshake) {
  CAF_LOG_TRACE(CAF_ARG(handshake));
  auto rp = self_->make_response_promise();
  return add_unchecked_outbound_path_impl(rp, std::move(handshake));
}

stream_slot stream_manager::add_unchecked_inbound_path_impl() {
  CAF_LOG_TRACE("");
  auto x = self_->current_mailbox_element();
  if (x == nullptr || !x->content().match_elements<open_stream_msg>()) {
    CAF_LOG_ERROR("add_unchecked_inbound_path called, but current message "
                  "is not an open_stream_msg");
    return invalid_stream_slot;
  }
  auto& osm = x->content().get_mutable_as<open_stream_msg>(0);
  if (out().terminal() && !self_->current_forwarding_stack().empty()) {
    // Sinks must always terminate the stream.
    CAF_LOG_WARNING("add_unchecked_inbound_path called in a sink, but the "
                    "handshake has further stages");
    stream_slots path_id{osm.slot, 0};
    auto code = sec::cannot_add_downstream;
    inbound_path::emit_irregular_shutdown(self_, path_id,
                                          std::move(osm.prev_stage), code);
    auto rp = self_->make_response_promise();
    rp.deliver(code);
    return invalid_stream_slot;
  }
  auto slot = assign_next_slot();
  stream_slots path_id{osm.slot, slot};
  auto ptr = self_->make_inbound_path(this, path_id, std::move(osm.prev_stage));
  CAF_ASSERT(ptr != nullptr);
  ptr->emit_ack_open(self_, actor_cast<actor_addr>(osm.original_stage));
  return slot;
}

stream_slot stream_manager::assign_next_slot() {
  return self_->assign_next_slot_to(this);
}

stream_slot stream_manager::assign_next_pending_slot() {
  return self_->assign_next_pending_slot_to(this);
}

void stream_manager::finalize(const error&) {
  // nop
}

message stream_manager::make_final_result() {
  return none;
}

error stream_manager::process_batch(message&) {
  CAF_LOG_ERROR("stream_manager::process_batch called");
  return sec::invalid_stream_state;
}

void stream_manager::output_closed(error) {
  // nop
}

void stream_manager::downstream_demand(outbound_path*, long) {
  CAF_LOG_ERROR("stream_manager::downstream_demand called");
}

void stream_manager::input_closed(error) {
  // nop
}

} // namespace caf
