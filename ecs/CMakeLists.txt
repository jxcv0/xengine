cmake_minimum_required(VERSION 3.16)
project(ecs)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -Wpedantic")
set(SOURCES
  ecs.c
)

add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC . )
# target_link_libraries(${PROJECT_NAME} PRIVATE)

add_subdirectory(test)
