link_libraries(resource GTest::gtest_main)

add_executable(meshtests meshtests.cpp)

include(GoogleTest)
gtest_discover_tests(meshtests)
