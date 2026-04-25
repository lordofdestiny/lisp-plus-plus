#pragma once

#include <cxxabi.h>

#include <string>

static std::string demangle(const char *name) {
  int status = -1;
  // abi::__cxa_demangle allocates memory that MUST be freed
  char *demangled = abi::__cxa_demangle(name, nullptr, nullptr, &status);

  if (status == 0) {
    std::string result(demangled);
    std::free(demangled); // Prevent memory leaks
    return result;
  }
  return name; // Return original if demangling fails
}
