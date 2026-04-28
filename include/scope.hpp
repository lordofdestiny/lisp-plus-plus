#pragma once

#include <tuple>

#include <filter.hpp>
#include <statement.hpp>
#include <util.hpp>
#include <variable/variable.hpp>

#include <demangle.hpp>

template<typename... Args>
struct scope : statement_base<scope<Args...>>
{
  scope(Args... args)
    : fns(args...)
    , variables()
  {
  }

  template<typename... FnArgs>
  void invoke_impl(FnArgs... args)
  {
    tuple_for_each(
      fns, [this]<size_t Index>(std::index_sequence<Index>, auto&& stat) {
        constexpr auto var_count =
          filter_index_sequence(
            std::index_sequence_for<Args...>{},
            [&]<size_t X>(std::index_sequence<X>) {
              return is_variable_v<std::tuple_element_t<X, decltype(fns)>> &&
                     X < Index;
            })
            .size();

        auto var_args = [&]<size_t... J>(std::index_sequence<J...> j) {
          return std::tie((std::get<J>(variables))...);
        }(std::make_index_sequence<var_count>{});

        if constexpr (is_variable_v<decltype(stat)>) {
          auto& target = std::get<var_count>(variables);
          std::apply(stat, std::make_tuple(std::ref(target), var_args));
        } else {
          std::apply(stat, var_args);
        }
      });
  }

  std::tuple<Args...> fns;
  variables<Args...> variables;
};
