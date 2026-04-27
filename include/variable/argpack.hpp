#pragma once

#include <type_traits> // IWYU pragma: keep

template<typename... Args>
struct ArgumentPack
{};

template<typename... Args>
struct first_pack_arg
{
  using type = ArgumentPack<>;
};

template<typename T, typename... Args>
struct first_pack_arg<T, Args...>
{
  using type = ArgumentPack<T>;
};

template<typename... Args>
using first_pack_arg_t = typename first_pack_arg<Args...>::type;

template<typename... Packs>
struct MergeArgumentPack;

template<>
struct MergeArgumentPack<>
{
  using type = ArgumentPack<>;
};

template<typename... Args>
struct MergeArgumentPack<ArgumentPack<Args...>>
{
  using type = ArgumentPack<Args...>;
};

template<typename... Args1, typename... Args2, typename... Packs>
struct MergeArgumentPack<ArgumentPack<Args1...>,
                         ArgumentPack<Args2...>,
                         Packs...>
{
  using type = typename MergeArgumentPack<ArgumentPack<Args1..., Args2...>,
                                          Packs...>::type;
};

template<typename... Packs>
using merge_argument_pack_t = typename MergeArgumentPack<Packs...>::type;
