link_libraries(resource GTest::gtest_main)

add_executable(meshtests meshtests.cpp)
add_executable(colladatests colladatests.cpp)

include(GoogleTest)
gtest_discover_tests(meshtests)
gtest_discover_tests(colladatests)
