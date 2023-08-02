cmake_minimum_required(VERSION 3.16)
project(assets)

set(SOURCES
	assets.h
	assets.c
)

add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC . )
target_link_libraries(${PROJECT_NAME} PRIVATE pthread rt stb assimp)

if (XENGINE_BUILD_TESTS)
  add_subdirectory(test)
endif()
