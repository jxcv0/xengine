cmake_minimum_required(VERSION 3.22.2)
project(ge_tests)

enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a")

set(SOURCES
    vec_tests.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PUBLIC ge gtest_main)

add_test(
    NAME ${PROJECT_NAME}
    COMMAND ${PROJECT_NAME}
)