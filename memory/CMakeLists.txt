cmake_minimum_required(VERSION 3.16)
project(memory)

set(
  SOURCES
    heap.cpp
)

set(
  HEADERS
    allocator.h
    heap.h
)

add_library(${PROJECT_NAME} SHARED ${HEADERS} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC .)
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(memory lin types)

# add_subdirectory(test)
