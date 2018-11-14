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

#include "caf/detail/parse_ini.hpp"

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

namespace caf {
namespace detail {

void parse_ini_t::operator()(std::istream& input, const config_consumer& consumer_fun,
                             opt_err errors) const {
  // wraps a temporary into an (lvalue) config_value and calls `consumer_fun`
  auto consumer = [&](size_t ln, std::string name, config_value x) {
    consumer_fun(ln, std::move(name), x, errors);
  };
  std::string group;
  std::string line;
  size_t ln = 0; // line number
  auto print = [&](const char* category, const char* str) {
    if (errors)
      *errors << category << " INI file line "
              << ln << ": " << str << std::endl;
  };
  auto print_error = [&](const char* str) {
    print("[ERROR]", str);
  };
  auto print_warning = [&](const char* str) {
    print("[WARNING]", str);
  };
  while (std::getline(input, line)) {
    ++ln;
    // get begin-of-line (bol) and end-of-line (eol), ignoring whitespaces
    auto eol = find_if_not(line.rbegin(), line.rend(), ::isspace).base();
    auto bol = find_if_not(line.begin(), eol, ::isspace);
    // ignore empty lines and lines starting with ';' (comment)
    if (bol == eol || *bol == ';')
      continue;
    // do we read a group name?
    if (*bol == '[') {
      if (*(eol - 1) != ']')
        print_error("missing ] at end of line");
      else
        group.assign(bol + 1, eol - 1);
      // skip further processing of this line
      continue;
    }
    // do we have a group name yet? (prohibit ungrouped values)
    if (group.empty()) {
      print_error("value outside of a group");
      continue;
    }
    // position of the equal sign
    auto eqs = find(bol, eol, '=');
    if (eqs == eol) {
      print_error("no '=' found");
      continue;
    }
    if (bol == eqs) {
      print_error("line starting with '='");
      continue;
    }
    if ((eqs + 1) == eol) {
      print_error("line ends with '='");
      continue;
    }
    // our keys have the format "<group>.<config-name>"
    auto key = group;
    key += '.';
    // ignore any whitespace between config-name and equal sign
    key.insert(key.end(), bol, find_if(bol, eqs, ::isspace));
    // begin-of-value, ignoring whitespaces after '='
    auto bov = find_if_not(eqs + 1, eol, ::isspace);
    // auto-detect what we are dealing with
    constexpr const char* true_str = "true";
    constexpr const char* false_str = "false";
    auto icase_eq = [](char x, char y) {
      return tolower(x) == tolower(y);
    };
    if (std::equal(bov, eol, true_str, icase_eq)) {
      consumer(ln, std::move(key), true);
    } else if (std::equal(bov, eol, false_str, icase_eq)) {
      consumer(ln, std::move(key), false);
    } else if (*bov == '\'') {
      // end-of-atom iterator
      auto eoa = eol - 1;
      if (bov == eoa) {
        print_error("stray '");
        continue;
      }
      if (*eoa != '\'') {
        print_error("atom not terminated by '");
        continue;
      }
      ++bov; // skip leading '
      if (std::distance(bov, eoa) > 10) {
        print_error("atoms are not allowed to have more than 10 characters");
        continue;
      }
      // found an atom value, copy it into a null-terminated buffer
      char buf[11];
      std::copy(bov, eoa, buf);
      buf[std::distance(bov, eoa)] = '\0';
      atom_value result = atom(buf);
      consumer(ln, std::move(key), result);
    } else if (*bov == '"') {
      // end-of-string iterator
      auto eos = eol - 1;
      if (bov == eos) {
        print_error(R"(stray '"')");
        continue;
      }
      if (*eos != '"') {
        print_error(R"(string not terminated by '"')");
        continue;
      }
      // found a string, remove first and last char from string,
      // start escaping string sequence
      auto last_char_backslash = false;
      std::string result;
      result.reserve(static_cast<size_t>(std::distance(bov, eos)));
      // skip leading " and iterate up to the trailing "
      ++bov;
      for (; bov != eos; ++bov) {
        if (last_char_backslash) {
          switch (*bov) {
            case 'n':
              result += '\n';
              break;
            case 't':
              result += '\t';
              break;
            default:
              result += *bov;
          }
          last_char_backslash = false;
        } else if (*bov == '\\') {
          last_char_backslash = true;
        } else {
          result += *bov;
        }
      }
      if (last_char_backslash)
        print_warning("trailing quotation mark escaped");
      consumer(ln, std::move(key), std::move(result));
    } else {
      bool is_neg = *bov == '-';
      if (is_neg && ++bov == eol) {
        print_error("'-' is not a number");
        continue;
      }
      auto set_ival = [&](int base, int prefix_len, const char* err) {
        auto advanced_bov = bov + prefix_len;
        const char* str_begin = &*(advanced_bov);
        const char* str_end = str_begin + std::distance(advanced_bov, eol);
        char* e;
        int64_t res = std::strtoll(str_begin, &e, base);
        // check if we reached the end
        if (e != str_end)
          print_error(err);
        else
          consumer(ln, std::move(key), is_neg ? -res : res);
      };
      auto set_dval = [&] {
        const char* str_begin = &*(bov);
        const char* str_end = str_begin + std::distance(bov, eol);
        char* e;
        double res = std::strtod(str_begin, &e);
        if (e != str_end)
          print_error("invalid value");
        else
          consumer(ln, std::move(key), is_neg ? -res : res);
      };
      if (*bov == '0' && std::distance(bov, eol) > 1)
        switch (*(bov + 1)) {
          case 'x':
          case 'X':
            set_ival(16, 2, "invalid hex value");
            break;
          case 'b':
          case 'B':
            set_ival(2, 2, "invalid binary value");
            break;
          default:
            if (all_of(bov, eol, ::isdigit))
              set_ival(8, 1, "invalid oct value");
            else
              set_dval();
        }
      else if (all_of(bov, eol, ::isdigit))
        set_ival(10, 0, "invalid decimal value");
      else
        set_dval();
    }
  }
}

} // namespace detail
} // namespace caf
