set(SOURCES
  chunk_tests.cpp
  # archetype_tests.cpp
  entity_mgr_tests.cpp
  # ecs_tests.cpp
  # component_array_tests.cpp
  # system_tests.cpp
  )

add_executable(ecs2_tests ${SOURCES})
target_link_libraries(ecs2_tests gtest_main ecs2)
add_test(NAME ecs2_tests COMMAND ecs2_tests)
