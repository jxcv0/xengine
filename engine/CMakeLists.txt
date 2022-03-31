cmake_minimum_required(VERSION 3.22.1)
project(run)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a")

add_executable(${PROJECT_NAME} run.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE ge)