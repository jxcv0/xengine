link_libraries(memory GTest::gtest_main)

add_executable(databasetests databasetests.cpp)

include(GoogleTest)
gtest_discover_tests(databasetests)
