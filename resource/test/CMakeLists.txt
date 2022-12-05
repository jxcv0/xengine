link_libraries(resource GTest::gtest_main)

add_executable(importtests importtests.cpp)
add_executable(resourcetests resourcetests.cpp)
add_executable(logallocatortests logallocatortests.cpp)
add_executable(mmapfiletests mmapfiletests.cpp)

include(GoogleTest)
gtest_discover_tests(importtests)
gtest_discover_tests(resourcetests)
gtest_discover_tests(logallocatortests)
gtest_discover_tests(mmapfiletests)

# add_executable(resource_perf perf.cpp)
