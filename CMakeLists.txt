cmake_minimum_required(VERSION 3.16)
project(xengine)

set(CMAKE_BUILD_PARALLEL_LEVEL 8)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -Werror -Wextra -Wpedantic")
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.13.0
)
FetchContent_MakeAvailable(googletest)

include(CTest)

add_subdirectory(lib)
add_subdirectory(assets)
add_subdirectory(ecs)
add_subdirectory(ecs2)
