cmake_minimum_required(VERSION 3.16)
project(assetmgr)

set(SOURCES
	assetmgr.c
)

add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC .)
target_link_libraries(${PROJECT_NAME} PRIVATE pthread rt)

add_executable(assetmgrd assetmgrd_main.c)
target_link_libraries(assetmgrd ${PROJECT_NAME} rt)

add_subdirectory(test)
