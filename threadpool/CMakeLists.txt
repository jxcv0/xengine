cmake_minimum_required(VERSION 3.16)
project(threadpool)

set(
  SOURCES
    threadpool.cpp
)

set(
  HEADERS
    threadpool.h
)

add_library(${PROJECT_NAME} SHARED ${HEADERS} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PUBLIC pthread)
target_include_directories(${PROJECT_NAME} PUBLIC .)

add_subdirectory(test)
