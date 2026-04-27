#pragma once

#include <functional>  // IWYU pragma: keep
#include <type_traits> // IWYU pragma: keep

#include <statement.hpp>
#include <variable/scope_var.hpp>
#include <variable/variable_impl.hpp>

template<typename... Args>
using variables = typename scope_var_types<Args...>::tuple_type;
