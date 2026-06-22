cmake_minimum_required(VERSION 3.28)

project(c4-sync-of-concurrent-operations LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(condition-variable condition-variable.cpp)