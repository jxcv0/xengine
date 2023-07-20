cmake_minimum_required(VERSION 3.16)
project(ecs2)

set(SOURCES
  types.hpp
  archetype.hpp
  archetype.cpp
  entity_mgr.hpp
  entity_mgr.cpp
  # ecs.hpp
  # system.h
)

add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC . )
target_link_libraries(${PROJECT_NAME} PRIVATE tbb) # -ltbb required for std::execution
target_compile_definitions(${PROJECT_NAME} PRIVATE -DPARALLEL)

add_subdirectory(test)
