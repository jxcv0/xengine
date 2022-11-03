cmake_minimum_required(VERSION 3.16)
project(lintests VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

link_libraries(lin GTest::gtest_main)

add_executable(vec2tests vec2tests.cpp)
add_executable(vec3tests vec3tests.cpp)
add_executable(vec4tests vec4tests.cpp)
add_executable(mat4tests mat4tests.cpp)
add_executable(lintests lintests.cpp)

include(GoogleTest)
gtest_discover_tests(vec3tests)
gtest_discover_tests(vec4tests)
gtest_discover_tests(mat4tests)
gtest_discover_tests(vec2tests)
gtest_discover_tests(lintests)
