cmake_minimum_required(VERSION 3.16)
project(types)

set(
    SOURCES
        texture.cpp
)

set(
    HEADERS
        model.h
        mesh.h
        material.h
        texture.h
)

add_library(${PROJECT_NAME} SHARED ${HEADERS} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC .)
target_link_libraries(${PROJECT_NAME} PUBLIC stb_image)
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
