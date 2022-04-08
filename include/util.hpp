#ifndef UTIL_HPP
#define UTIL_HPP

#include "glad.h"
#include "camera.hpp"
#include <GLFW/glfw3.h>

namespace xen {

    /**
     * @brief Frambuffer size callback function
     * 
     * @param window the window
     * @param width the new width
     * @param height the new height
     */
    void framebuffer_size_callback(GLFWwindow *window, int width, int height);

    /**
     * @brief Process input
     * 
     * @param window
     */
    void process_input(GLFWwindow *window);
} // namespace xen::util

#endif