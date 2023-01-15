cmake_minimum_required(VERSION 3.16)
project(tools VERSION 0.1.0)

add_executable(rig rig.c)
add_executable(collada collada.c)

target_link_libraries(rig PUBLIC glad render input resource stb)

target_include_directories(collada PUBLIC /usr/include/libxml2)
target_link_libraries(collada PUBLIC xml2 resource)
