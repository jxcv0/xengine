link_libraries(ecs GTest::gtest_main)

add_executable(entityarraytests entityarraytests.cpp)
add_executable(componentarraytests componentarraytests.cpp)

include(GoogleTest)
gtest_discover_tests(entityarraytests)
gtest_discover_tests(componentarraytests)
