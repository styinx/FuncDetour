cmake_minimum_required(VERSION 3.21)

project(FuncDetour)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BINARY_DIR CMAKE_SOURCE_DIR)

include_directories(include)

add_executable(example examples/example.cpp)
