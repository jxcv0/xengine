cmake_minimum_required(VERSION 3.16)
project(ecs)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -Wpedantic")
set(SOURCES
  ecs.hpp
)

add_library(${PROJECT_NAME} STATIC ${SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX) 
target_include_directories(${PROJECT_NAME} PUBLIC . )
# target_link_libraries(${PROJECT_NAME} PRIVATE)

if (XENGINE_BUILD_TESTS)
  add_subdirectory(test)
endif()
