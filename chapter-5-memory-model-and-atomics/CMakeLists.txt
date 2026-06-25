cmake_minimum_required(VERSION 3.28)

project(c5-memory-model-and-atomics LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(is-lock-free is-lock-free.cpp)
