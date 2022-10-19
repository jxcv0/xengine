cmake_minimum_required(VERSION 3.16)
project(threadpool)

set(
    SOURCES
)

set(
    HEADERS
        threadpool.h
        i_task.h
)

add_library(${PROJECT_NAME} SHARED ${HEADERS} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC .)
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
