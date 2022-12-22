cmake_minimum_required(VERSION 3.16)
project(ecs)

set(
  SOURCES
)

set(
  HEADERS
    constants.h
    entityarray.h
    componentarray.h
    archetype.h
    archetypearray.h
    ecs.h
)

add_library(${PROJECT_NAME} SHARED ${HEADERS} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC .)
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(ecs threadpool lin resource render)

add_subdirectory(test)
