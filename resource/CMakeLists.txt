cmake_minimum_required(VERSION 3.16)
project(resource)

set(
  SOURCES
    mmapfile.c
    texture.cpp
    mesh.cpp
)

set(
  HEADERS
    mmapfile.h
    mesh.h
    material.h
    texture.h
)

add_library(${PROJECT_NAME} SHARED ${HEADERS} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC .)

target_link_libraries(${PROJECT_NAME} PUBLIC memory render glad lin stb_image)

add_subdirectory(test)
