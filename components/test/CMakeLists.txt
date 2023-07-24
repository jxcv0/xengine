set(SOURCES
  component_table_tests.cpp
  )

add_executable(components_tests ${SOURCES})
target_link_libraries(components_tests gtest_main components)
add_test(NAME components_tests COMMAND components_tests)
