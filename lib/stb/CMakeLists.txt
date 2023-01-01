cmake_minimum_required(VERSION 3.16)
project(stb)

set(
    SOURCES
        stb_image.h
        stb_truetype.h
)

set(
    HEADERS
        stb_image.c
        stb_truetype.c
)

add_library(${PROJECT_NAME} STATIC ${HEADERS} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC .)
