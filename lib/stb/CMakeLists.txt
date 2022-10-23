cmake_minimum_required(VERSION 3.16)
project(stb_image)

set(
    SOURCES
        stb_image.h
)

set(
    HEADERS
        stb_image.cpp
)

add_library(${PROJECT_NAME} SHARED ${HEADERS} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC .)
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
