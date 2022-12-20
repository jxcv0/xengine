cmake_minimum_required(VERSION 3.16)
project(xengineexample VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

add_executable(example main.cpp)

target_link_libraries(example glad render resource ecs)
