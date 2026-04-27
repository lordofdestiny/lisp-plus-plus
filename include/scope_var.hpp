#pragma once

#include "variable.hpp"

template <typename... Args> struct ArgumentPack {};

template <typename... Args> struct first_pack_arg {
  using type = ArgumentPack<>;
};

template <typename T, typename... Args> struct first_pack_arg<T, Args...> {
  using type = ArgumentPack<T>;
};

template <typename... Args>
using first_pack_arg_t = first_pack_arg<Args...>::type;

template <typename... Packs> struct MergeArgumentPack;

template <> struct MergeArgumentPack<> {
  using type = ArgumentPack<>;
};

template <typename... Args> struct MergeArgumentPack<ArgumentPack<Args...>> {
  using type = ArgumentPack<Args...>;
};

template <typename... Args1, typename... Args2, typename... Packs>
struct MergeArgumentPack<ArgumentPack<Args1...>, ArgumentPack<Args2...>,
                         Packs...> {
  using type =
      MergeArgumentPack<ArgumentPack<Args1..., Args2...>, Packs...>::type;
};

template <typename... Packs>
using merge_argument_pack_t = MergeArgumentPack<Packs...>::type;

template <typename T, typename Predicate>
auto FilterOneArgumentPack(ArgumentPack<T>, Predicate p) {
  if constexpr (p(ArgumentPack<T>{})) {
    return ArgumentPack<T>{};
  } else {
    return ArgumentPack<>{};
  }
}

template <typename... Args, typename Predicate>
auto filter_argument_pack(ArgumentPack<Args...>, Predicate p) {
  return MergeArgumentPack<decltype(FilterOneArgumentPack(ArgumentPack<Args>{},
                                                          p))...>{};
}

template <typename... Args> struct scope_var_types;

template <typename PrevPack, size_t I, typename T, bool is_variable,
          typename... Rest>
struct scope_var_impl_type;

template <typename PrevPack, size_t I, typename T, typename... Rest>
struct scope_var_impl_type<PrevPack, I, T, false, Rest...> {
  scope_var_impl_type(T t) {}

  static constexpr size_t Index = 0;
  static constexpr size_t Size = 0;

  using tuple_type = std::tuple<>;
};

template <typename F, typename Pack> struct VariableType;

template <typename F, typename... Args>
struct VariableType<F, ArgumentPack<Args...>> {
  using type = std::invoke_result_t<F, Args...>;
};

template <typename PrevPack, size_t I, typename T, typename... Rest>
struct scope_var_impl_type<PrevPack, I, T, true, Rest...> {
  scope_var_impl_type(T t) {}

  static constexpr size_t Index = I;
  static constexpr size_t Size = 1;

  // using variable_calls = decltype(filter_argument_pack(
  //     PrevPack{},
  //     []<typename P>(ArgumentPack<P>) { return is_variable_v<P>; }));
  // using value_type =
  //   std::conditional_t<
  //     std::is_invocable_v<typename T::fn_type>,
  //     std::invoke_result_t<typename T::fn_type>,
  //     std::invoke_result_t<typename T::fn_type, PrevPack>
  //   >;
  // PrevPack must be filtered, also mapped to return values
  using value_type = std::invoke_result_t<typename T::fn_type>;
  using tuple_type = std::tuple<value_type>;

  value_type value;
};

template <typename Pack, size_t N, typename... Args>
struct scope_var_impl_types {
  scope_var_impl_types(Args...) {}
  using tuple_type = std::tuple<>;
};

template <typename PrevPack, size_t N, typename T, typename... Rest>
using parent_type_first = scope_var_impl_type<PrevPack, N - sizeof...(Rest) - 1,
                                              T, T::is_variable, Rest...>;

template <typename PrevPack, size_t N, typename T, typename... Rest>
using parent_type_rest =
    scope_var_impl_types<merge_argument_pack_t<PrevPack, ArgumentPack<T>>, N,
                         Rest...>;

template <typename PrevPack, size_t N, typename T, typename... Rest>
struct scope_var_impl_types<PrevPack, N, T, Rest...>
    : parent_type_first<PrevPack, N, T, Rest...>,
      parent_type_rest<PrevPack, N, T, Rest...> {

  using parent_type_first = parent_type_first<PrevPack, N, T, Rest...>;
  using parent_type_rest = parent_type_rest<PrevPack, N, T, Rest...>;

  scope_var_impl_types(T t, Rest... rest)
      : parent_type_first(std::move(t)), parent_type_rest(std::move(rest)...) {}

  using tuple_type = decltype(std::tuple_cat(
      std::declval<typename parent_type_first::tuple_type>(),
      std::declval<typename parent_type_rest::tuple_type>()));

  static constexpr size_t Size = std::tuple_size<tuple_type>();
};

template <typename... Args>
using scope_var_impl =
    scope_var_impl_types<ArgumentPack<>, sizeof...(Args), Args...>;

template <typename... Args> struct scope_var_types : scope_var_impl<Args...> {

  scope_var_types(Args... args) : scope_var_impl<Args...>(std::move(args)...) {}

  using parent_tuple_type = scope_var_impl<Args...>;

  static constexpr size_t Size =
      std::tuple_size<typename parent_tuple_type::tuple_type>();
};
