#pragma once

#include <tuple>

#include <filter.hpp>
#include <statement.hpp>
#include <variable/variable.hpp>

template<typename... Args>
struct scope : statement_base<scope<Args...>>
{
  scope(Args... args)
    : fns(args...)
    , variables()
  {
  }

  void invoke_impl()
  {
    [this]<size_t... I>(std::index_sequence<I...>) {
      // Invoking all statements
      (
        [&]<size_t Index>(std::index_sequence<Index>) {
          // Index is statement index
          // Invoking single statement
          auto var_indices = filter_index_sequence(
            std::index_sequence_for<Args...>{},
            [&]<size_t X>(std::index_sequence<X>) {
              return std::tuple_element_t<X, decltype(fns)>::is_variable &&
                     X < Index;
            });

          auto& stat = std::get<Index>(fns);

          auto var_args = [&]<size_t... J>(std::index_sequence<J...> j) {
            return std::tie((std::get<J>(variables))...);
          }(std::make_index_sequence<var_indices.size()>{});

          if constexpr (std::remove_reference_t<decltype(stat)>::is_variable) {
            auto& target = std::get<var_indices.size()>(variables);
            std::apply(stat, std::make_tuple(std::ref(target), var_args));
          } else {
            std::apply(stat, var_args);
          }
        }(std::index_sequence<I>{}),
        ...);
    }(std::index_sequence_for<Args...>{});
  }

  std::tuple<Args...> fns;
  variables<Args...> variables;
};
