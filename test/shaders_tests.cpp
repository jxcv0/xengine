#include <gtest/gtest.h>

#include "shaders.hpp"

TEST(ShadersTests, read_shader_file_test) {
    const char *p_expected = 
    "#version 460 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "\n"
    "void main() {\n"
    "    gl_Position = vec4(aPos, 1.0);\n"
    "}";

    std::string expected(p_expected);

    std::string actual = xen::shaders::read_shader_file("assets/shaders/test.vert");

    ASSERT_EQ(actual, expected);
}