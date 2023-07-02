cmake_minimum_required(VERSION 3.16)
project(assetmgr)

set(SOURCES
	src/assetmgr.c
)

add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} PRIVATE pthread)

add_executable(assetmgr-d src/assetmgr-main.c)
target_link_libraries(assetmgr-d ${PROJECT_NAME})

add_subdirectory(test)