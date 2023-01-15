cmake_minimum_required(VERSION 3.16)
project(resource)

set(
  SOURCES
    mesh.c
)

set(
  HEADERS
    mesh.h
)

add_library(${PROJECT_NAME} STATIC ${HEADERS} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC .)

target_link_libraries(${PROJECT_NAME} PUBLIC utils lin stb)

add_subdirectory(test)
