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

#include "caf/io/network/manager.hpp"

#include "caf/logger.hpp"

#include "caf/io/abstract_broker.hpp"

namespace caf {
namespace io {
namespace network {

manager::manager() : parent_(nullptr) {
  // nop
}

manager::~manager() {
  // nop
}

void manager::set_parent(abstract_broker* ptr) {
  parent_ = ptr != nullptr ? ptr->ctrl() : nullptr;
}

abstract_broker* manager::parent() {
  return parent_ ? static_cast<abstract_broker*>(parent_->get()) : nullptr;
}

void manager::detach(execution_unit*, bool invoke_disconnect_message) {
  CAF_LOG_TRACE("");
  if (!detached()) {
    CAF_LOG_DEBUG("disconnect servant from broker");
    auto raw_ptr = parent();
    // keep the strong reference until we go out of scope
    strong_actor_ptr ptr;
    ptr.swap(parent_);
    detach_from(raw_ptr);
    if (invoke_disconnect_message) {
      auto mptr = make_mailbox_element(nullptr, invalid_message_id,
                                       {}, detach_message());
      switch (raw_ptr->consume(*mptr)) {
        case im_success:
          raw_ptr->finalize();
          break;
        case im_skipped:
          raw_ptr->push_to_cache(std::move(mptr));
          break;
        case im_dropped:
          CAF_LOG_INFO("broker dropped disconnect message");
          break;
      }
    }
  }
}

} // namespace network
} // namespace io
} // namespace caf
