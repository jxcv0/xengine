cmake_minimum_required(VERSION 3.16)
project(xengine VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

add_executable(placeholdertest placeholdertest.cpp)

target_link_libraries(placeholdertest GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(placeholdertest)
