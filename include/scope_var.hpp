#pragma once

template <typename... Args> struct scope_var_types;

template <size_t I, typename T, bool is_variable, typename... Rest>
struct scope_var_impl_type;

template <size_t I, typename T, typename... Rest>
struct scope_var_impl_type<I, T, false, Rest...> {
  scope_var_impl_type(T t) {}

  static constexpr size_t Index = 0;
  static constexpr size_t Size = 0;

  using tuple_type = std::tuple<>;
};

template <size_t I, typename T, typename... Rest>
struct scope_var_impl_type<I, T, true, Rest...> {
  scope_var_impl_type(T t) {}

  static constexpr size_t Index = I;
  static constexpr size_t Size = 1;

  using value_type = std::invoke_result_t<typename T::fn_type>;
  using tuple_type = std::tuple<value_type>;

  value_type value;
};

template <size_t N, typename... Args> struct scope_var_impl_types {
  scope_var_impl_types(Args...) {}
  using tuple_type = std::tuple<>;
};

template <size_t N, typename T, typename... Rest>
using parent_type_first =
    scope_var_impl_type<N - sizeof...(Rest) - 1, T, T::is_variable, Rest...>;

template <size_t N, typename T, typename... Rest>
using parent_type_rest = scope_var_impl_types<N, Rest...>;

template <size_t N, typename T, typename... Rest>
struct scope_var_impl_types<N, T, Rest...> : parent_type_first<N, T, Rest...>,
                                             parent_type_rest<N, T, Rest...> {

  using parent_type_first = parent_type_first<N, T, Rest...>;
  using parent_type_rest = parent_type_rest<N, T, Rest...>;

  scope_var_impl_types(T t, Rest... rest)
      : parent_type_first(std::move(t)), parent_type_rest(std::move(rest)...) {}

  using tuple_type = decltype(std::tuple_cat(
      std::declval<typename parent_type_first::tuple_type>(),
      std::declval<typename parent_type_rest::tuple_type>()));

  static constexpr size_t Size = std::tuple_size<tuple_type>();
};

template <typename... Args>
using scope_var_impl = scope_var_impl_types<sizeof...(Args), Args...>;

template <typename... Args> struct scope_var_types : scope_var_impl<Args...> {

  scope_var_types(Args... args) : scope_var_impl<Args...>(std::move(args)...) {}

  using parent_tuple_type = scope_var_impl<Args...>;

  static constexpr size_t Size =
      std::tuple_size<typename parent_tuple_type::tuple_type>();
};
