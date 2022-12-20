cmake_minimum_required(VERSION 3.16)
project(render)

set(
  SOURCES
    window.cpp
    shader.cpp
    checkerr.cpp
)

set(
  HEADERS
    window.h
    shader.h
    checkerr.h
    camera.h
)

add_library(${PROJECT_NAME} SHARED ${HEADERS} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC .)
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(render glad glfw lin)
