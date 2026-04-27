#pragma once

#include <scope.hpp>
#include <statement.hpp>

template<typename... F>
struct program : statement_base<program<F...>>
{
  program(F... statements) { (statements(), ...); }
};