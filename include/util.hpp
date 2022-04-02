#ifndef UTIL_HPP
#define UTIL_HPP

#include "glad.h"
#include <GLFW/glfw3.h>

namespace util {
    void framebuffer_size_callback(GLFWwindow *window, int width, int height);

    void process_input(GLFWwindow *window);
} // namespace util

#endif