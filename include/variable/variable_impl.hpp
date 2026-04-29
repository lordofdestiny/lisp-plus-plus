#pragma once

template<typename Enabler, typename F, typename... Args>
struct NoArgumentInvocable : std::true_type
{};

template<typename F, typename... Args>
struct NoArgumentInvocable<std::void_t<std::invoke_result_t<F, Args...>>,
                           F,
                           Args...> : std::false_type
{};

template<typename F, typename... Args>
constexpr bool no_argument_invocable_v =
  NoArgumentInvocable<void, F, Args...>::value;

template<typename T, typename = void>
struct is_variable
{
  static constexpr bool value = false;
};

template<typename T>
struct is_variable<T, std::void_t<decltype(T::is_variable)>>
{
  static constexpr bool value = T::is_variable;
};

template<typename T>
static constexpr bool is_variable_v =
  is_variable<std::remove_cvref_t<T>>::value;

#include <demangle.hpp>
#include <iostream>

template<typename T>
struct variable : statement_base<variable<T>>
{
  static constexpr bool is_variable = true;
  variable(T arg) {};

  using fn_type = T;

  template<typename... Args>
  void invoke_impl(auto& _value, std::tuple<Args...>&& args)
  {
    if constexpr (no_argument_invocable_v<T, Args...>) {
      _value = std::invoke(fn);
    } else {
      _value = std::apply(fn, std::forward<decltype(args)>(args));
    }
  }

  T fn;
};
