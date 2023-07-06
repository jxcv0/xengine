add_executable(assets_tests assets_tests.cpp)
target_link_libraries(assets_tests gtest_main assets)
add_test(NAME assets_tests COMMAND assets_tests)
