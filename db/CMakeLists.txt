cmake_minimum_required(VERSION 3.16)
project(db)

set(
  SOURCES
    id_list.c
)

set(
  HEADERS
    id_list.h
)

add_library(${PROJECT_NAME} STATIC ${HEADERS} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC .)

# target_link_libraries(${PROJECT_NAME} PUBLIC)

add_subdirectory(test)
