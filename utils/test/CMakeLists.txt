link_libraries(resource GTest::gtest_main)

add_executable(mmapfiletests mmapfiletests.cpp)

include(GoogleTest)
gtest_discover_tests(mmapfiletests)
