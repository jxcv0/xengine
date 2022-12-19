link_libraries(ecs GTest::gtest_main)

add_executable(entityarraytests entityarraytests.cpp)
add_executable(archetypetests archetypetests.cpp)
add_executable(archetypearraytests archetypearraytests.cpp)
add_executable(coordinatortests coordinatortests.cpp)
add_executable(ecstests ecstests.cpp)

include(GoogleTest)
gtest_discover_tests(entityarraytests)
gtest_discover_tests(archetypetests)
gtest_discover_tests(archetypearraytests)
gtest_discover_tests(coordinatortests)
gtest_discover_tests(ecstests)
