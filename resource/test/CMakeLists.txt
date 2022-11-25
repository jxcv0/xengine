link_libraries(resource GTest::gtest_main)

add_executable(importertests importertests.cpp)
add_executable(resourcetests resourcetests.cpp)
add_executable(logallocatortests logallocatortests.cpp)

include(GoogleTest)
gtest_discover_tests(importertests)
gtest_discover_tests(resourcetests)
gtest_discover_tests(logallocatortests)
