cmake_minimum_required(VERSION 3.16)
project(threadpool)

set(
    SOURCES
)

set(
    HEADERS
        threadpool.h
        task.h
)

add_library(${PROJECT_NAME} SHARED ${HEADERS} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PUBLIC pthread)
target_include_directories(${PROJECT_NAME} PUBLIC .)
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

add_executable(profile test/profile.cpp)
target_include_directories(profile PUBLIC .)
target_link_libraries(profile PRIVATE ${PROJECT_NAME})
