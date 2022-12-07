cmake_minimum_required(VERSION 3.16)
project(resource)

set(
    SOURCES
        import.cpp
        mmapfile.cpp
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
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(${PROJECT_NAME} PUBLIC memory render glad lin types)

add_subdirectory(test)
