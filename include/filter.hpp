#pragma once

#include <utility>

template<size_t I, typename Predicate>
constexpr auto
filter_single(std::index_sequence<I>, Predicate pred)
{
  if constexpr (pred(std::index_sequence<I>{})) {
    return std::index_sequence<I>{};
  } else {
    return std::index_sequence<>{};
  }
}

template<size_t... s, size_t... t>
constexpr std::index_sequence<s..., t...>
concat_sequences(std::index_sequence<s...>, std::index_sequence<t...>)
{
  return {};
}

template<size_t... s, size_t... t, typename... R>
constexpr auto
concat_sequences(std::index_sequence<s...>, std::index_sequence<t...>, R...)
{
  return concat_sequences(std::index_sequence<s..., t...>{}, R{}...);
}

constexpr std::index_sequence<>
concat_sequences(std::index_sequence<>)
{
  return {};
}

template<size_t... I, typename Predicate>
constexpr auto
filter_index_sequence(std::index_sequence<I...>, Predicate pred)
{
  if constexpr (sizeof...(I) > 0) {
    return concat_sequences(filter_single(std::index_sequence<I>{}, pred)...);
  }
}
