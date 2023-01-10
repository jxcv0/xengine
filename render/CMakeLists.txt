cmake_minimum_required(VERSION 3.16)
project(render)

set(
  SOURCES
    window.c
    shader.c
    checkerr.c
    camera.c
    text.c
    render.c
    deferred_render.c
)

set(
  HEADERS
    window.h
    shader.h
    checkerr.h
    camera.h
    text.h
    render.h
    deferred_render.h
)

add_library(${PROJECT_NAME} STATIC ${HEADERS} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC .)

target_link_libraries(${PROJECT_NAME} PUBLIC glad glfw lin utils resource stb m)
