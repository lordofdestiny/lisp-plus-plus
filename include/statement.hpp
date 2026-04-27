#pragma once

#include <functional>
#include <utility>

template<typename T>
struct statement_base
{
  static constexpr bool is_variable = false;

  template<typename... Args>
  void operator()(Args&&... args)
  {
    return static_cast<T&>(*this).invoke_impl(std::forward<Args>(args)...);
  }
};

template<typename T>
struct statement : statement_base<statement<T>>
{
  static constexpr bool is_variable = false;

  statement(T arg) {};

  template<typename U = T, typename... Args>
  void invoke_impl(Args... args)
  {
    std::invoke(fn, args...);
  }

  T fn;
};