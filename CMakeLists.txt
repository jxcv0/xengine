cmake_minimum_required(VERSION 3.18)
project(xengine VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -Dcpp_variable")

enable_testing()

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(glm REQUIRED)
find_package(assimp REQUIRED)

set(
  SOURCES 
    include/window.h
    include/checkerr.h
)

add_library(${PROJECT_NAME} STATIC ${SOURCES}) 
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(
  ${PROJECT_NAME}
    PUBLIC
      ${PROJECT_SOURCE_DIR}/include
      ${PROJECT_SOURCE_DIR}/lib/stb
)

target_link_libraries(
  ${PROJECT_NAME}
    PUBLIC
      glfw
      glad
      OpenGL::GL
      assimp
)

add_subdirectory(lib/glad)
add_subdirectory(xengine)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
