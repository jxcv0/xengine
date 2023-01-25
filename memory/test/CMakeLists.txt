link_libraries(memory GTest::gtest_main)

add_executable(id_list_tests id_list_tests.cpp)

include(GoogleTest)
gtest_discover_tests(id_list_tests)
