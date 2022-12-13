link_libraries(ecs GTest::gtest_main)

add_executable(entityarraytests entityarraytests.cpp)
add_executable(componentarraytests componentarraytests.cpp)
add_executable(coordinatortests coordinatortests.cpp)

include(GoogleTest)
gtest_discover_tests(entityarraytests)
gtest_discover_tests(componentarraytests)
gtest_discover_tests(coordinatortests)
