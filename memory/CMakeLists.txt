cmake_minimum_required(VERSION 3.16)
project(memory)

set(
    SOURCES
        heap.h
)

set(
    HEADERS
        allocator.h
      	heap.cpp
)

add_library(${PROJECT_NAME} SHARED ${HEADERS} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC .)
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
