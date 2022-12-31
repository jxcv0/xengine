link_libraries(lin GTest::gtest_main)

add_executable(lintests lintests.cpp)

include(GoogleTest)
gtest_discover_tests(lintests)
