#pragma once

#include "statement.hpp"

template <typename T,
          std::enable_if_t<!std::is_same_v<std::invoke_result_t<T>, void>,
                           bool> = true>
struct variable : statement_base<variable<T>> {
  static constexpr bool is_variable = true;
  variable(T arg) {};

  using fn_type = T;

  void invoke_impl(auto &_value) { _value = std::invoke(fn); }

  T fn;
};
