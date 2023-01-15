cmake_minimum_required(VERSION 3.16)
project(tools VERSION 0.1.0)

add_executable(rig rig.c)
add_executable(collada collada.cpp)

target_link_libraries(rig PUBLIC glad render input resource stb)

# this will end in tears
target_include_directories(collada PUBLIC /usr/include/collada-dom2.5)
target_link_libraries(collada PUBLIC collada-dom2.5-dp) 
