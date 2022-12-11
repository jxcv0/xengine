link_libraries(resource GTest::gtest_main)

add_executable(meshtests meshtests.cpp)
add_executable(mmapfiletests mmapfiletests.cpp)

include(GoogleTest)
gtest_discover_tests(meshtests)
gtest_discover_tests(mmapfiletests)

# add_executable(resource_perf perf.cpp)
