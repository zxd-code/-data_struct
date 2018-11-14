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

#ifndef CAF_ARG_WRAPPER_HPP
#define CAF_ARG_WRAPPER_HPP

namespace caf {
namespace detail {

/// Enables automagical string conversion for `CAF_ARG`.
template <class T>
struct arg_wrapper {
  const char* name;
  const T& value;
  arg_wrapper(const char* x, const T& y) : name(x), value(y) {
    // nop
  }
};

/// Used to implement `CAF_ARG`.
template <class T>
static arg_wrapper<T> make_arg_wrapper(const char* name, const T& value) {
  return {name, value};
}

} // namespace detail
} // namespace caf

#endif // CAF_ARG_WRAPPER_HPP
