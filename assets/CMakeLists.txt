cmake_minimum_required(VERSION 3.16)
project(assets)

set(SOURCES
	assets.c
)

add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC . )
target_link_libraries(${PROJECT_NAME} PRIVATE pthread rt stb assimp)

add_subdirectory(test)
