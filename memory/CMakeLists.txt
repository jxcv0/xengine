cmake_minimum_required(VERSION 3.16)
project(memory)

set(
  SOURCES
    database.c
)

set(
  HEADERS
    database.h
)

add_library(${PROJECT_NAME} STATIC ${HEADERS} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC .)

target_link_libraries(${PROJECT_NAME} PUBLIC )

add_subdirectory(test)
