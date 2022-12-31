cmake_minimum_required(VERSION 3.16)
project(lin)

set(
  SOURCES
    lin.c
)

set(
  HEADERS
    lin.h
)

add_library(${PROJECT_NAME} STATIC ${HEADERS} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC .)
target_link_libraries(lin m)

add_subdirectory(test)
