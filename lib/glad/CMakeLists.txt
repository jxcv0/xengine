cmake_minimum_required(VERSION 3.16)
project(glad)

set(
    SOURCES
        src/glad.c
)

set(
    HEADERS
        include/glad/glad.h
        include/KHR/khrplatform.h
)

add_library(${PROJECT_NAME} STATIC ${HEADERS} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC include/glad/)
