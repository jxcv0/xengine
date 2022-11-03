cmake_minimum_required(VERSION 3.16)
project(xenginetests VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

link_libraries(xengine GTest::gtest_main)

add_executable(poolallocatortests poolallocatortests.cpp)
add_executable(allocatortests allocatortests.cpp)

include(GoogleTest)
gtest_discover_tests(poolallocatortests)
gtest_discover_tests(allocatortests)
