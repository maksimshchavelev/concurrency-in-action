project(c3-shared-data LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(data-race data-race.cpp)
add_executable(mutex mutex.cpp)
add_executable(race-condition race-condition.cpp)
add_executable(deadlock deadlock.cpp)
add_executable(lock lock.cpp)
add_executable(deadlock-without-mutex deadlock-without-mutex.cpp)
add_executable(hierarchical-mutex hierarchical-mutex.cpp)
add_executable(unique-lock unique-lock.cpp)
add_executable(call-once call-once.cpp)

target_compile_options(data-race PRIVATE -fsanitize=thread -g)
target_link_options(data-race PRIVATE -fsanitize=thread)

target_compile_options(mutex PRIVATE -g)
