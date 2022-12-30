cmake_minimum_required(VERSION 3.16)
project(lin)

set(
  SOURCES
    lin.c
)

set(
  HEADERS
    # vec2.h
    # vec3.h
    # vec4.h
    # mat4.h
    lin.h
)

add_library(${PROJECT_NAME} STATIC ${HEADERS} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC .)

add_subdirectory(test)
