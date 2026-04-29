#pragma once

#include <functional> // IWYU pragma: keep
#include <type_traits>

#include <variable/argpack.hpp>
#include <variable/variable_impl.hpp>

template<typename Pack, typename T>
using append_if_var =
  std::conditional_t<is_variable_v<T>,
                     merge_argument_pack_t<Pack, ArgumentPack<T>>,
                     Pack>;

template<typename... Packs>
struct UnpackStatementArgs;

template<typename... Args>
using unpack_statements_t = typename UnpackStatementArgs<Args...>::type;

template<typename... T>
struct UnpackStatementArgs<ArgumentPack<T...>>
{
  using type = ArgumentPack<>;
};

template<typename T>
struct UnpackStatementArgs<ArgumentPack<variable<T>>>
{
  using type = ArgumentPack<std::invoke_result_t<T>&>;
};

template<typename First, typename... Rest>
struct UnpackStatementArgs<ArgumentPack<variable<First>, variable<Rest>...>>
{
  using current_type = std::invoke_result_t<First>&;
  using type = merge_argument_pack_t<
    ArgumentPack<current_type>,
    unpack_statements_t<ArgumentPack<current_type>, variable<Rest>...>>;
};

template<typename... Front, typename Current, typename... Back>
struct UnpackStatementArgs<ArgumentPack<Front...>,
                           variable<Current>,
                           variable<Back>...>
{
  using current_type = std::invoke_result_t<Current, Front...>&;
  using merged_pack = ArgumentPack<Front..., current_type>;

  using type =
    merge_argument_pack_t<ArgumentPack<current_type>,
                          unpack_statements_t<merged_pack, variable<Back>...>>;
};

template<typename F, typename Pack>
struct VariableType;

template<typename F, typename... Args>
struct VariableType<F, ArgumentPack<Args...>>
{
  using type = std::invoke_result_t<F, Args...>;
};

template<typename F>
struct VariableType<F, ArgumentPack<>>
{
  using type = std::invoke_result_t<F>;
};

template<typename F, typename ArgsPack>
using variable_type_t = typename VariableType<F, ArgsPack>::type;

template<typename... Args>
struct scope_var_types;

template<typename PrevPack,
         size_t I,
         typename T,
         bool is_variable,
         typename... Rest>
struct scope_var_impl_type;

template<typename PrevPack, size_t I, typename T, typename... Rest>
struct scope_var_impl_type<PrevPack, I, T, false, Rest...>
{
  scope_var_impl_type(T t) {}

  static constexpr size_t Index = 0;
  static constexpr size_t Size = 0;

  using tuple_type = std::tuple<>;
};

template<typename PrevPack, size_t I, typename T, typename... Rest>
struct scope_var_impl_type<PrevPack, I, T, true, Rest...>
{
  static constexpr size_t Index = I;
  static constexpr size_t Size = 1;

  // PrevPack must be filtered, also mapped to return values
  using value_type =
    variable_type_t<typename T::fn_type, unpack_statements_t<PrevPack>>;
  using tuple_type = std::tuple<value_type>;

  value_type value;
};

template<typename Pack, size_t N, typename... Args>
struct scope_var_impl_types
{
  using tuple_type = std::tuple<>;
};

template<typename PrevPack, size_t N, typename T, typename... Rest>
using parent_type_first = scope_var_impl_type<PrevPack,
                                              N - sizeof...(Rest) - 1,
                                              T,
                                              T::is_variable,
                                              Rest...>;

template<typename PrevPack, size_t N, typename T, typename... Rest>
using parent_type_rest =
  scope_var_impl_types<append_if_var<PrevPack, T>, N, Rest...>;

template<typename PrevPack, size_t N, typename T, typename... Rest>
struct scope_var_impl_types<PrevPack, N, T, Rest...>
  : parent_type_first<PrevPack, N, T, Rest...>
  , parent_type_rest<PrevPack, N, T, Rest...>
{
  using parent_type_first_t = parent_type_first<PrevPack, N, T, Rest...>;
  using parent_type_rest_t = parent_type_rest<PrevPack, N, T, Rest...>;

  using tuple_type = decltype(std::tuple_cat(
    std::declval<typename parent_type_first_t::tuple_type>(),
    std::declval<typename parent_type_rest_t::tuple_type>()));

  static constexpr size_t Size = std::tuple_size<tuple_type>();
};

template<typename... Args>
using scope_var_impl =
  scope_var_impl_types<ArgumentPack<>, sizeof...(Args), Args...>;

template<typename... Args>
struct scope_var_types : scope_var_impl<Args...>
{
  using parent_tuple_type = scope_var_impl<Args...>;

  static constexpr size_t Size =
    std::tuple_size<typename parent_tuple_type::tuple_type>();
};
