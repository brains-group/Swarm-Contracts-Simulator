cmake_minimum_required(VERSION 3.20) 
project(swarm_contracts_simulator LANGUAGES CXX)

set(BUILD_TESTING OFF CACHE BOOL "Enable building tests")
include(CTest)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -Wpedantic -Werror -Wno-unused -g -fdiagnostics-color=always)

add_subdirectory(src)

