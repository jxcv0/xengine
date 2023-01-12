link_libraries(resource GTest::gtest_main)

add_executable(mmapfiletests mmapfiletests.cpp)
add_executable(xmlparsertests xmlparsertests.cpp)

include(GoogleTest)
gtest_discover_tests(mmapfiletests)
gtest_discover_tests(xmlparsertests)
