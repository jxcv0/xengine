cmake_minimum_required(VERSION 3.16)
project(xengine VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
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
find_package(glm REQUIRED)
find_package(assimp REQUIRED)

add_subdirectory(lib/glad)
add_subdirectory(lib/stb)
add_subdirectory(game)
add_subdirectory(threadpool)
add_subdirectory(memory)
add_subdirectory(resource)
add_subdirectory(ecs)
add_subdirectory(lin)
add_subdirectory(render)
add_subdirectory(input)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
