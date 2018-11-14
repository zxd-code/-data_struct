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

#define CAF_SUITE continuous_streaming

#include "caf/test/dsl.hpp"

#include <memory>
#include <numeric>

#include "caf/actor_system.hpp"
#include "caf/actor_system_config.hpp"
#include "caf/event_based_actor.hpp"
#include "caf/stateful_actor.hpp"

using std::string;

using namespace caf;

namespace {

TESTEE_SETUP();

VARARGS_TESTEE(file_reader, size_t buf_size) {
  using buf = std::deque<int>;
  return {
    [=](string& fname) -> output_stream<int, string> {
      CAF_CHECK_EQUAL(fname, "numbers.txt");
      CAF_CHECK_EQUAL(self->mailbox().empty(), true);
      return self->make_source(
        // forward file name in handshake to next stage
        std::forward_as_tuple(std::move(fname)),
        // initialize state
        [=](buf& xs) {
          xs.resize(buf_size);
          std::iota(xs.begin(), xs.end(), 1);
        },
        // get next element
        [](buf& xs, downstream<int>& out, size_t num) {
          CAF_MESSAGE("push " << num << " messages downstream");
          auto n = std::min(num, xs.size());
          for (size_t i = 0; i < n; ++i)
            out.push(xs[i]);
          xs.erase(xs.begin(), xs.begin() + static_cast<ptrdiff_t>(n));
        },
        // check whether we reached the end
        [=](const buf& xs) {
          if (xs.empty()) {
            CAF_MESSAGE(self->name() << " is done");
            return true;
          }
          return false;
        });
    }
  };
}

TESTEE_STATE(sum_up) {
  int x = 0;
};

TESTEE(sum_up) {
  return {
    [=](stream<int>& in, const string& fname) {
      CAF_CHECK_EQUAL(fname, "numbers.txt");
      using int_ptr = int*;
      return self->make_sink(
        // input stream
        in,
        // initialize state
        [=](int_ptr& x) {
          x = &self->state.x;
        },
        // processing step
        [](int_ptr& x , int y) {
          *x += y;
        },
        // cleanup
        [=](int_ptr&, const error&) {
          CAF_MESSAGE(self->name() << " is done");
        }
      );
    },
    [=](join_atom atm, actor src) {
      CAF_MESSAGE(self->name() << " joins a stream");
      self->send(self * src, atm);
    }
  };
}

TESTEE_STATE(stream_multiplexer) {
  stream_stage_ptr<int, broadcast_downstream_manager<int>> stage;
};

TESTEE(stream_multiplexer) {
  self->state.stage = self->make_continuous_stage(
    // initialize state
    [](unit_t&) {
      // nop
    },
    // processing step
    [](unit_t&, downstream<int>& out, int x) {
      out.push(x);
    },
    // cleanup
    [=](unit_t&, const error&) {
      CAF_MESSAGE(self->name() << " is done");
    }
  );
  return {
    [=](join_atom) {
      CAF_MESSAGE("received 'join' request");
      return self->state.stage->add_outbound_path(
        std::make_tuple("numbers.txt"));
    },
    [=](const stream<int>& in, std::string& fname) {
      CAF_CHECK_EQUAL(fname, "numbers.txt");
      return self->state.stage->add_inbound_path(in);
    },
  };
}

using fixture = test_coordinator_fixture<>;

} // namespace <anonymous>

// -- unit tests ---------------------------------------------------------------

CAF_TEST_FIXTURE_SCOPE(local_streaming_tests, fixture)

CAF_TEST(depth_3_pipeline_with_fork) {
  auto src = sys.spawn(file_reader, 50);
  auto stg = sys.spawn(stream_multiplexer);
  auto snk1 = sys.spawn(sum_up);
  auto snk2 = sys.spawn(sum_up);
  auto& st = deref<stream_multiplexer_actor>(stg).state;
  CAF_MESSAGE("connect sinks to the stage (fork)");
  self->send(snk1, join_atom::value, stg);
  self->send(snk2, join_atom::value, stg);
  sched.run();
  CAF_CHECK_EQUAL(st.stage->out().num_paths(), 2u);
  CAF_MESSAGE("connect source to the stage (fork)");
  self->send(stg * src, "numbers.txt");
  sched.run();
  CAF_CHECK_EQUAL(st.stage->out().num_paths(), 2u);
  CAF_CHECK_EQUAL(st.stage->inbound_paths().size(), 1u);
  sched.run_dispatch_loop(streaming_cycle);
  CAF_CHECK_EQUAL(st.stage->out().num_paths(), 2u);
  CAF_CHECK_EQUAL(st.stage->inbound_paths().size(), 0u);
  CAF_CHECK_EQUAL(deref<sum_up_actor>(snk1).state.x, 1275);
  CAF_CHECK_EQUAL(deref<sum_up_actor>(snk2).state.x, 1275);
  self->send_exit(stg, exit_reason::kill);
}

CAF_TEST(depth_3_pipeline_with_join) {
  auto src1 = sys.spawn(file_reader, 50);
  auto src2 = sys.spawn(file_reader, 50);
  auto stg = sys.spawn(stream_multiplexer);
  auto snk = sys.spawn(sum_up);
  auto& st = deref<stream_multiplexer_actor>(stg).state;
  CAF_MESSAGE("connect sink to the stage");
  self->send(snk, join_atom::value, stg);
  sched.run();
  CAF_CHECK_EQUAL(st.stage->out().num_paths(), 1u);
  CAF_MESSAGE("connect sources to the stage (join)");
  self->send(stg * src1, "numbers.txt");
  self->send(stg * src2, "numbers.txt");
  sched.run();
  CAF_CHECK_EQUAL(st.stage->out().num_paths(), 1u);
  CAF_CHECK_EQUAL(st.stage->inbound_paths().size(), 2u);
  sched.run_dispatch_loop(streaming_cycle);
  CAF_CHECK_EQUAL(st.stage->out().num_paths(), 1u);
  CAF_CHECK_EQUAL(st.stage->inbound_paths().size(), 0u);
  CAF_CHECK_EQUAL(deref<sum_up_actor>(snk).state.x, 2550);
  self->send_exit(stg, exit_reason::kill);
}

CAF_TEST_FIXTURE_SCOPE_END()
