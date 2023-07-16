cmake_minimum_required(VERSION 3.16)
project(ecs2)

set(SOURCES
  archetype.hpp
  # entity_mgr.cpp
  # entity_mgr.h
  # component_array.h
  # system.h
  # ecs.h
)

add_library(${PROJECT_NAME} STATIC ${SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(${PROJECT_NAME} PUBLIC . )
target_link_libraries(${PROJECT_NAME} PRIVATE tbb) # -ltbb required for std::execution
target_compile_definitions(${PROJECT_NAME} PRIVATE -DPARALLEL)

add_subdirectory(test)
