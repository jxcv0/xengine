cmake_minimum_required(VERSION 3.16)
project(tools VERSION 0.1.0)

add_executable(rig rig.c)
add_executable(converter converter.c)

target_link_libraries(rig PUBLIC glad render input resource stb)

target_link_libraries(converter PUBLIC assimp resource)
