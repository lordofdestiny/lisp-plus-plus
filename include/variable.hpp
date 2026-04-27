#pragma once

#include "statement.hpp"

template <typename T> struct variable : statement_base<variable<T>> {
  static constexpr bool is_variable = true;
  variable(T arg) {};

  using fn_type = T;

  template <typename... Args> void invoke_impl(auto &_value, Args &&...args) {
    _value = std::apply(fn, std::forward<Args>(args)...);
  }

  T fn;
};

template <typename T, typename = void> struct is_variable {
  static constexpr bool value = false;
};

template <typename T>
struct is_variable<T, std::void_t<decltype(T::is_variable)>> {
  static constexpr bool value = T::is_variable;
};

template <typename T>
static constexpr bool is_variable_v = is_variable<T>::value;