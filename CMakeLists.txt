cmake_minimum_required(VERSION 3.22.1)
project(ge VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a")

enable_testing()

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

include(FetchContent)

FetchContent_Declare(
  googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        c9461a9b55ba954df0489bab6420eb297bed846b
)
FetchContent_MakeAvailable(googletest)

find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)

set(
  SOURCES
    src/vec.cpp
)

set(
  HEADERS
    include/vec.hpp
)

add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})

target_include_directories(
  ${PROJECT_NAME}
    PUBLIC
      ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(
  ${PROJECT_NAME}
    PUBLIC
      glad
      glfw
      OpenGL::GL
)

add_subdirectory(lib/glad)
add_subdirectory(test)
add_subdirectory(engine)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)