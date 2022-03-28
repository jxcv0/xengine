cmake_minimum_required(VERSION 3.22.1)
project(ge VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a")

enable_testing()

include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        c9461a9b55ba954df0489bab6420eb297bed846b
)
FetchContent_MakeAvailable(googletest)

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

add_subdirectory(test)