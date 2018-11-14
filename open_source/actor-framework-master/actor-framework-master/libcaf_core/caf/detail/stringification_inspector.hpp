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

#ifndef CAF_DETAIL_STRINGIFICATION_INSPECTOR_HPP
#define CAF_DETAIL_STRINGIFICATION_INSPECTOR_HPP

#include <chrono>
#include <functional>
#include <string>
#include <type_traits>
#include <vector>

#include "caf/atom.hpp"
#include "caf/none.hpp"
#include "caf/error.hpp"

#include "caf/meta/type_name.hpp"
#include "caf/meta/omittable.hpp"
#include "caf/meta/annotation.hpp"
#include "caf/meta/hex_formatted.hpp"
#include "caf/meta/omittable_if_none.hpp"
#include "caf/meta/omittable_if_empty.hpp"

#include "caf/detail/int_list.hpp"
#include "caf/detail/apply_args.hpp"
#include "caf/detail/type_traits.hpp"

namespace caf {
namespace detail {

class stringification_inspector {
public:
  // -- member types required by Inspector concept -----------------------------

  using result_type = void;

  static constexpr bool reads_state = true;

  // -- constructors, destructors, and assignment operators --------------------

  stringification_inspector(std::string& result) : result_(result) {
    // nop
  }

  // -- operator() -------------------------------------------------------------

  template <class... Ts>
  void operator()(Ts&&... xs) {
    traverse(std::forward<Ts>(xs)...);
  }

  /// Prints a separator to the result string.
  void sep();

  inline void traverse() noexcept {
    // end of recursion
  }

  void consume(atom_value& x);

  void consume(const char* cstr);

  void consume_hex(const uint8_t* xs, size_t n);

  inline void consume(bool& x) {
    result_ += x ? "true" : "false";
  }

  inline void consume(char* cstr) {
    consume(const_cast<const char*>(cstr));
  }

  inline void consume(std::string& str) {
    consume(str.c_str());
  }

  template <class T>
  enable_if_tt<std::is_arithmetic<T>> consume(T& x) {
    result_ += std::to_string(x);
  }

  // unwrap std::ref
  template <class T>
  void consume(std::reference_wrapper<T>& x) {
    return consume(x.get());
  }

  /// Picks up user-defined `to_string` functions.
  template <class T>
  enable_if_t<!std::is_pointer<T>::value && has_to_string<T>::value>
  consume(T& x) {
    result_ += to_string(x);
  }

  /// Delegates to `inspect(*this, x)` if available and `T`
  /// does not provide a `to_string`.
  template <class T>
  enable_if_t<
    is_inspectable<stringification_inspector, T>::value
    && !has_to_string<T>::value>
  consume(T& x) {
    inspect(*this, x);
  }

  template <class F, class S>
  void consume(std::pair<F, S>& x) {
    result_ += '(';
    traverse(deconst(x.first), deconst(x.second));
    result_ += ')';
  }

  template <class... Ts>
  void consume(std::tuple<Ts...>& x) {
    result_ += '(';
    apply_args(*this, get_indices(x), x);
    result_ += ')';
  }

  template <class T>
  enable_if_t<is_iterable<T>::value
              && !is_inspectable<stringification_inspector, T>::value
              && !has_to_string<T>::value>
  consume(T& xs) {
    result_ += '[';
    // use a hand-written for loop instead of for-each to avoid
    // range-loop-analysis warnings when using this function with vector<bool>
    for (auto i = xs.begin(); i != xs.end(); ++i) {
      sep();
      consume(deconst(*i));
    }
    result_ += ']';
  }

  template <class T>
  enable_if_t<has_peek_all<T>::value
              && !is_iterable<T>::value // pick begin()/end() over peek_all
              && !is_inspectable<stringification_inspector, T>::value
              && !has_to_string<T>::value>
  consume(T& xs) {
    result_ += '[';
    xs.peek_all(*this);
    result_ += ']';
  }

  template <class T>
  void consume(T* xs, size_t n) {
    result_ += '(';
    for (size_t i = 0; i < n; ++i) {
      sep();
      consume(deconst(xs[i]));
    }
    result_ += ')';
  }

  template <class T, size_t S>
  void consume(std::array<T, S>& xs) {
    return consume(xs.data(), S);
  }

  template <class T, size_t S>
  void consume(T (&xs)[S]) {
    return consume(xs, S);
  }

  template <class T>
  enable_if_tt<std::is_pointer<T>> consume(T ptr) {
    if (ptr) {
      result_ += '*';
      consume(*ptr);
    } else {
      result_ += "<null>";
    }
  }

  /// Print duration types with nanosecond resolution.
  template <class Rep>
  void consume(std::chrono::duration<Rep, std::nano>& x) {
    result_ += std::to_string(x.count());
    result_ += "ns";
  }

  /// Print duration types with microsecond resolution.
  template <class Rep>
  void consume(std::chrono::duration<Rep, std::micro>& x) {
    result_ += std::to_string(x.count());
    result_ += "us";
  }

  /// Print duration types with millisecond resolution.
  template <class Rep>
  void consume(std::chrono::duration<Rep, std::milli>& x) {
    result_ += std::to_string(x.count());
    result_ += "ms";
  }

  /// Print duration types with second resolution.
  template <class Rep>
  void consume(std::chrono::duration<Rep, std::ratio<1>>& x) {
    result_ += std::to_string(x.count());
    result_ += "s";
  }

  /// Print duration types with minute resolution.
  template <class Rep>
  void consume(std::chrono::duration<Rep, std::ratio<60>>& x) {
    result_ += std::to_string(x.count());
    result_ += "min";
  }

  /// Print duration types with hour resolution.
  template <class Rep>
  void consume(std::chrono::duration<Rep, std::ratio<3600>>& x) {
    result_ += std::to_string(x.count());
    result_ += "h";
  }

  /// Fallback printing `<unprintable>`.
  template <class T>
  enable_if_t<
    !is_iterable<T>::value
    && !has_peek_all<T>::value
    && !std::is_pointer<T>::value
    && !is_inspectable<stringification_inspector, T>::value
    && !std::is_arithmetic<T>::value
    && !has_to_string<T>::value>
  consume(T&) {
    result_ += "<unprintable>";
  }

  template <class T, class... Ts>
  void traverse(meta::hex_formatted_t, T& x, Ts&&... xs) {
    sep();
    consume_hex(reinterpret_cast<uint8_t*>(deconst(x).data()), x.size());
    traverse(std::forward<Ts>(xs)...);
  }

  template <class T, class... Ts>
  void traverse(meta::omittable_if_none_t, T& x, Ts&&... xs) {
    if (x != none) {
      sep();
      consume(x);
    }
    traverse(std::forward<Ts>(xs)...);
  }

  template <class T, class... Ts>
  void traverse(meta::omittable_if_empty_t, T& x, Ts&&... xs) {
    if (!x.empty()) {
      sep();
      consume(x);
    }
    traverse(std::forward<Ts>(xs)...);
  }

  template <class T, class... Ts>
  void traverse(meta::omittable_t, T&, Ts&&... xs) {
    traverse(std::forward<Ts>(xs)...);
  }

  template <class... Ts>
  void traverse(meta::type_name_t x, Ts&&... xs) {
    sep();
    result_ += x.value;
    result_ += '(';
    traverse(std::forward<Ts>(xs)...);
    result_ += ')';
  }

  template <class... Ts>
  void traverse(const meta::annotation&, Ts&&... xs) {
    traverse(std::forward<Ts>(xs)...);
  }

  template <class T, class... Ts>
  enable_if_t<!meta::is_annotation<T>::value> traverse(T&& x, Ts&&... xs) {
    sep();
    consume(deconst(x));
    traverse(std::forward<Ts>(xs)...);
  }

private:
  template <class T>
  T& deconst(const T& x) {
    return const_cast<T&>(x);
  }

  std::string& result_;
};

} // namespace detail
} // namespace caf

#endif // CAF_DETAIL_STRINGIFICATION_INSPECTOR_HPP
