#pragma once

template <size_t I, typename T, bool = T::is_variable>
struct scope_var_impl_type {
  using StatementFunctionType = std::tuple<>;
  scope_var_impl_type(T t) {}

  static constexpr size_t Index = 0;
  static constexpr size_t Size = 0;

  std::tuple<> to_tuple_impl() { return {}; }
};

template <size_t I, typename T> struct scope_var_impl_type<I, T, true> {
  scope_var_impl_type(T t) {}

  static constexpr size_t Index = I;
  static constexpr size_t Size = 1;
  using value_type = typename T::var_type;
  using StatementFunctionType = std::tuple<value_type>;

  std::tuple<value_type> to_tuple_impl() { return value; }

  value_type value;
};

template <size_t N, typename... Args> struct scope_var_impl_types {
  using StatementFunctionType = std::tuple<>;
  scope_var_impl_types(Args...) {}

  std::tuple<> to_tuple_impl() { return {}; }
};

template <size_t N, typename T, typename... Rest>
struct scope_var_impl_types<N, T, Rest...>
    : scope_var_impl_type<N - sizeof...(Rest) - 1, T>,
      scope_var_impl_types<N, Rest...> {

  scope_var_impl_types(T t, Rest... rest)
      : scope_var_impl_type<N - sizeof...(Rest) - 1, T>(std::move(t)),
        scope_var_impl_types<N, Rest...>(std::move(rest)...) {}

  template <size_t... I>
  scope_var_impl_types(std::tuple<T, Rest...> args, std::index_sequence<I...>)
      : scope_var_impl_types(std::move(std::get<I>(args))...) {}

  using StatementFunctionTypesFirst =
      scope_var_impl_type<N - sizeof...(Rest) - 1, T>::StatementFunctionType;
  using StatementFunctionTypesRest =
      scope_var_impl_types<N, Rest...>::StatementFunctionType;

  using StatementFunctionType =
      decltype(std::tuple_cat(std::declval<StatementFunctionTypesFirst>(),
                              std::declval<StatementFunctionTypesRest>()));

  static constexpr size_t Size =
      std::tuple_size<StatementFunctionTypesFirst>() +
      std::tuple_size<StatementFunctionTypesFirst>();

  auto to_tuple_impl() {
    return std::tuple_cat(
        scope_var_impl_type<N - sizeof...(Rest) - 1, T>::to_tuple_impl(),
        scope_var_impl_types<N, Rest...>::to_tuple_impl());
  }
};

template <typename... Args>
struct scope_var_types : scope_var_impl_types<sizeof...(Args), Args...> {
  static constexpr size_t N = sizeof...(Args);
  scope_var_types(Args... args)
      : scope_var_impl_types<N, Args...>(std::move(args)...) {}

  scope_var_types(std::tuple<Args...> args)
      : scope_var_impl_types<N, Args...>(std::move(args),
                                         std::index_sequence_for<Args...>{}) {}

  using StatementFunctionType =
      scope_var_impl_types<N, Args...>::StatementFunctionType;

  static constexpr size_t Size = std::tuple_size<StatementFunctionType>();

  auto to_tuple() { return scope_var_impl_types<N, Args...>::to_tuple_impl(); }
};
