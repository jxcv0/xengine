cmake_minimum_required(VERSION 3.16)
project(render)

set(
  SOURCES
    shader.cpp
)

set(
  HEADERS
    mainwindow.h
    shader.h
)

add_library(${PROJECT_NAME} SHARED ${HEADERS} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC .)
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(render glad glfw lin)
