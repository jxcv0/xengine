cmake_minimum_required(VERSION 3.16)
project(resource)

set(
    SOURCES
        importer.cpp
        texture.cpp
)

set(
    HEADERS
        resource.h
        importer.h
        model.h
        mesh.h
        material.h
        texture.h
        vertex.h
)

add_library(${PROJECT_NAME} SHARED ${HEADERS} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC .)
target_link_libraries(${PROJECT_NAME} PUBLIC stb_image render glad)
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

add_subdirectory(test)
