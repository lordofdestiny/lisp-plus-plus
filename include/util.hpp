#pragma once

template<typename... Args>
constexpr void
tuple_for_each(std::tuple<Args...>& tup, auto&& fun)
{
  [&]<size_t... I>(std::index_sequence<I...>) {
    (
      [&]<size_t Index>(std::index_sequence<Index>) {
        fun(std::index_sequence<Index>{}, std::get<Index>(tup));
      }(std::index_sequence<I>{}),
      ...);
  }(std::index_sequence_for<Args...>{});
}
