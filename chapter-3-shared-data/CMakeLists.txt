project(c3-shared-data LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(data-race data-race.cpp)

target_compile_options(data-race PRIVATE -fsanitize=thread -g)
target_link_options(data-race PRIVATE -fsanitize=thread)


