cmake_minimum_required(VERSION 3.16)
project(resource)

set(
  SOURCES
    mmapfile.cpp
    texture.cpp
    mesh.cpp
)

set(
  HEADERS
    resource.h
    mmapfile.h
    mesh.h
    material.h
    texture.h
)

add_library(${PROJECT_NAME} SHARED ${HEADERS} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC .)
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(${PROJECT_NAME} PUBLIC memory render glad lin stb_image)

add_subdirectory(test)
