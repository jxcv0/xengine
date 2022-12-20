link_libraries(threadpool GTest::gtest_main)

add_executable(threadpooltests threadpooltests.cpp)

include(GoogleTest)
gtest_discover_tests(threadpooltests)
