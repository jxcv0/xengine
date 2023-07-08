set(SOURCES
  assets_tests.cpp
  mesh_alloc_tests.cpp
  )

add_executable(assets_tests ${SOURCES})
target_link_libraries(assets_tests gtest_main assets)
add_test(NAME assets_tests COMMAND assets_tests)
