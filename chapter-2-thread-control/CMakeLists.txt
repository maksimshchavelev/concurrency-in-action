cmake_minimum_required(VERSION 3.28)

project(c2-thread-control LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(background-task background-task.cpp)
add_executable(dangling-reference dangling-reference.cpp)
add_executable(join-exceptions join-exceptions.cpp)
add_executable(arguments-ub arguments-ub.cpp)
add_executable(method-call method-call.cpp)
add_executable(hardware-concurrency hardware-concurrency.cpp)

target_compile_options(dangling-reference PRIVATE -fsanitize=address,undefined -g)
target_link_options(dangling-reference PRIVATE -fsanitize=address,undefined)

target_compile_options(arguments-ub PRIVATE -fsanitize=address,undefined -g)
target_link_options(arguments-ub PRIVATE -fsanitize=address,undefined)