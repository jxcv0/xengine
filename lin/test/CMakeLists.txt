link_libraries(lin GTest::gtest_main)

add_executable(vec2tests vec2tests.cpp)
add_executable(vec3tests vec3tests.cpp)
add_executable(vec4tests vec4tests.cpp)
add_executable(mat4tests mat4tests.cpp)
add_executable(lintests lintests.cpp)

include(GoogleTest)
gtest_discover_tests(vec3tests)
gtest_discover_tests(vec4tests)
gtest_discover_tests(mat4tests)
gtest_discover_tests(vec2tests)
gtest_discover_tests(lintests)
