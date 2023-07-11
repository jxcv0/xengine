cmake_minimum_required(VERSION 3.16)
project(ecs2)

set(SOURCES
  entity_mgr.cpp
  entity_mgr.h
  component_array.cpp
  component_array.h
)

add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC . )
target_link_libraries(${PROJECT_NAME} PRIVATE)

add_subdirectory(test)
