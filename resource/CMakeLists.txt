cmake_minimum_required(VERSION 3.16)
project(resource)

set(
    SOURCES
        import.cpp
)

set(
    HEADERS
        resource.h
        import.h
        logallocator.h
        mmapfile.h
)

add_library(${PROJECT_NAME} SHARED ${HEADERS} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC .)
target_link_libraries(${PROJECT_NAME} PUBLIC render glad lin types)
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

add_subdirectory(test)
