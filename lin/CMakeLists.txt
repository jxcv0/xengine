cmake_minimum_required(VERSION 3.16)
project(lin)

set(
  SOURCES
)

set(
  HEADERS
    vec2.h
    vec3.h
    vec4.h
	  mat4.h
    lin.h
)

add_library(${PROJECT_NAME} SHARED ${HEADERS} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC .)
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
