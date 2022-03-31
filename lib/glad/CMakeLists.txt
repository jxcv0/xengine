cmake_minimum_required(VERSION 3.18.4)
project(Glad)

set(
    SOURCES
        src/glad.c
)

set(
    HEADERS
        include/glad/glad.h
        include/KHR/khrplatform.h
)

add_library(glad SHARED ${HEADERS} ${SOURCES})
target_include_directories(glad PUBLIC include/glad/)