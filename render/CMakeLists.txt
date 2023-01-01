cmake_minimum_required(VERSION 3.16)
project(render)

set(
  SOURCES
    window.c
    shader.c
    checkerr.c
    camera.c
)

set(
  HEADERS
    window.h
    shader.h
    checkerr.h
    camera.h
)

add_library(${PROJECT_NAME} STATIC ${HEADERS} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC .)

target_link_libraries(render glad glfw lin utils stb m)
