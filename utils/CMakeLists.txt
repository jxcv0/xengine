cmake_minimum_required(VERSION 3.16)
project(utils)

set(
  SOURCES
    utils.c
    mmapfile.c
    xmlparser.c
)

set(
  HEADERS
    utils.h
    mmapfile.h
    xmlparser.h
)

add_library(${PROJECT_NAME} STATIC ${HEADERS} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC .)

add_subdirectory(test)
