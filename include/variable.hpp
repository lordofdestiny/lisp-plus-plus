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

// Primary template: defaults to false_type
template <typename T, typename = void> struct is_variable {
  static constexpr bool value = false;
};

// Specialization: if T::value exists, decltype is valid and this is preferred
template <typename T> struct is_variable<T, std::void_t<decltype(T::value)>> {
  static constexpr bool value = T::is_variable;
};

template <typename T>
static constexpr bool is_variable_v = is_variable<T>::value;