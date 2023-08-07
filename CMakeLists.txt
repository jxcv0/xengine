cmake_minimum_required(VERSION 3.16)
project(xengine)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -Wall -Werror -Wextra -Wpedantic")
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_CXX_FLAGS_DEBUG "-ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

include(FetchContent)

if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  set(XENGINE_BUILD_TESTS TRUE)
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.13.0
  )
  FetchContent_MakeAvailable(googletest)
  include(CTest)
endif()

FetchContent_Declare(
  la
  GIT_REPOSITORY https://github.com/jxcv0/la.git
  GIT_TAG 6951edcf5ddb919873f4d9fa37dc9499737c508b
)

FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.3.8
)

FetchContent_MakeAvailable(la)
FetchContent_MakeAvailable(glfw)

add_subdirectory(lib)
add_subdirectory(assets)
add_subdirectory(ecs)
add_subdirectory(components)
