cmake_minimum_required(VERSION 3.16)
project(game VERSION 0.1.0)

add_executable(game main.cpp)

target_link_libraries(game glad render input resource ecs)
