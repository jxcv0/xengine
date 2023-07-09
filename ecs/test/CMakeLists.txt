set(SOURCES
  ecs_tests.cpp
  )

add_executable(ecs_tests ${SOURCES})
target_link_libraries(ecs_tests gtest_main ecs)
add_test(NAME ecs_tests COMMAND ecs_tests)
