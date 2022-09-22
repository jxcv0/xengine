cmake_minimum_required(VERSION 3.16)
project(xenginetests VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(TEST_COMMON_LIBS xengine GTest::gtest_main)

add_executable(entitysubsystem_tests entitysubsystem_tests.cpp)

target_link_libraries(entitysubsystem_tests ${TEST_COMMON_LIBS})

include(GoogleTest)
gtest_discover_tests(entitysubsystem_tests)
