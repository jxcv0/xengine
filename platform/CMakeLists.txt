cmake_minimum_required(VERSION 3.16)
project(platform)

# message("DISPLAY env: $ENV{DISPLAY}")
if (DEFINED ENV{DISPLAY})
  message(STATUS "XEN: Using X11")
  add_definitions(-DXEN_X11)
else()
  message(STATUS "XEN: Using GLFW")
endif()

set(
  SOURCES
    platform.c
)

set(
  HEADERS
    platform.h
)

add_library(${PROJECT_NAME} STATIC ${HEADERS} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC .)
target_link_libraries(lin -lX11)

# add_subdirectory(test)
