set(SOURCES
  tables_tests.cpp
  )

add_executable(tables_tests ${SOURCES})
target_link_libraries(tables_tests gtest_main tables)
add_test(NAME tables_tests COMMAND tables_tests)
