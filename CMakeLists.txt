cmake_minimum_required(VERSION 3.16)
project(xengine VERSION 0.1.0)

set(CMAKE_BUILD_TYPE debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
add_compile_options(-Wall -Wextra -Werror -fopenmp)

enable_testing()

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)

add_subdirectory(lib/glad)
add_subdirectory(lib/stb)
add_subdirectory(game)
add_subdirectory(threadpool)
add_subdirectory(resource)
add_subdirectory(lin)
add_subdirectory(render)
add_subdirectory(input)
add_subdirectory(utils)
add_subdirectory(tools)
add_subdirectory(platform)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
