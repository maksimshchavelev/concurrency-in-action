cmake_minimum_required(VERSION 3.28)

project(c1-hello-concurrent-world LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(hello-world hello-world.cpp)