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

#include "caf/outbound_path.hpp"

#include "caf/local_actor.hpp"
#include "caf/logger.hpp"
#include "caf/no_stages.hpp"
#include "caf/send.hpp"

namespace caf {

outbound_path::outbound_path(stream_slot sender_slot,
                             strong_actor_ptr receiver_hdl)
    : slots(sender_slot, invalid_stream_slot),
      hdl(std::move(receiver_hdl)),
      next_batch_id(1),
      open_credit(0),
      desired_batch_size(50),
      next_ack_id(1) {
  // nop
}

outbound_path::~outbound_path() {
  // nop
}

void outbound_path::emit_open(local_actor* self, stream_slot slot,
                              strong_actor_ptr to, message handshake_data,
                              stream_priority prio) {
  CAF_LOG_TRACE(CAF_ARG(slot) << CAF_ARG(to) << CAF_ARG(handshake_data)
                << CAF_ARG(prio));
  CAF_ASSERT(self != nullptr);
  CAF_ASSERT(to != nullptr);
  // Make sure we receive errors from this point on.
  stream_aborter::add(to, self->address(), slot,
                      stream_aborter::sink_aborter);
  // Send message.
  unsafe_send_as(self, to,
                 open_stream_msg{slot, std::move(handshake_data), self->ctrl(),
                                 nullptr, prio});
}

void outbound_path::emit_batch(local_actor* self, long xs_size, message xs) {
  CAF_LOG_TRACE(CAF_ARG(slots) << CAF_ARG(xs_size) << CAF_ARG(xs));
  CAF_ASSERT(open_credit >= xs_size);
  open_credit -= xs_size;
  auto bid = next_batch_id++;
  downstream_msg::batch batch{static_cast<int32_t>(xs_size), std::move(xs),
                              bid};
  unsafe_send_as(self, hdl,
                 downstream_msg{slots, self->address(), std::move(batch)});
}

void outbound_path::emit_regular_shutdown(local_actor* self) {
  CAF_LOG_TRACE(CAF_ARG(slots));
  unsafe_send_as(self, hdl,
                 make<downstream_msg::close>(slots, self->address()));
}

void outbound_path::emit_irregular_shutdown(local_actor* self, error reason) {
  CAF_LOG_TRACE(CAF_ARG(slots) << CAF_ARG(reason));
  unsafe_send_as(self, hdl,
                 make<downstream_msg::forced_close>(slots, self->address(),
                                                    std::move(reason)));
}

void outbound_path::emit_irregular_shutdown(local_actor* self,
                                            stream_slots slots,
                                            const strong_actor_ptr& hdl,
                                            error reason) {
  CAF_LOG_TRACE(CAF_ARG(slots) << CAF_ARG(hdl) << CAF_ARG(reason));
  unsafe_send_as(self, hdl,
                 make<downstream_msg::forced_close>(slots, self->address(),
                                                    std::move(reason)));
}

} // namespace caf
