cmake_minimum_required(VERSION 3.16)
project(xengine VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

enable_testing()

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(glm REQUIRED)
# find_package(assimp REQUIRED)

set(
  SOURCES 
    include/keys.h
    include/mainwindow.h
    include/shader.h
    include/shaderexceptions.h
)

set(
  HEADERS 
    src/main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS}) 
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
      glm::glm
      glad
      OpenGL::GL
      dl
      # assimp
)

add_subdirectory(lib/glad)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

