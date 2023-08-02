cmake_minimum_required(VERSION 3.16)
project(xengine)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -Wall -Werror -Wextra -Wpedantic")
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.13.0
)

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

FetchContent_MakeAvailable(googletest)
FetchContent_MakeAvailable(la)
FetchContent_MakeAvailable(glfw)

include(CTest)

add_subdirectory(lib)
add_subdirectory(assets)
add_subdirectory(tables)
