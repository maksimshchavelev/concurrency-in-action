cmake_minimum_required(VERSION 3.28)

project(c5-memory-model-and-atomics LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(is-lock-free is-lock-free.cpp)
add_executable(atomic-flag atomic-flag.cpp)
add_executable(spinlock-mutex spinlock-mutex.cpp)
add_executable(my-type-with-atomic my-type-with-atomic.cpp)

target_link_libraries(my-type-with-atomic PRIVATE -latomic)
