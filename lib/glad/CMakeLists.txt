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

add_library(${PROJECT_NAME} SHARED ${HEADERS} ${SOURCES})
target_include_directories(glad PUBLIC include/glad/)
target_link_libraries(glad dl)
