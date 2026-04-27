cmake_minimum_required(VERSION 3.24)
project(Lambda++ LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(main)

target_sources(main PRIVATE main.cpp)

target_include_directories(main PRIVATE include)

target_compile_options(main PRIVATE -include all.hpp)
