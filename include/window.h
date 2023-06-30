#ifndef WINDOW_H_
#define WINDOW_H_

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

/**
 * @brief Open a window and create an OpenGL context.
 *
 * @param width The width of the window.
 * @param height The height of the window.
 * @param A pointer the the new window.
 */
GLFWwindow *create_window(const float width, const float height,
                          const char *name);

#endif  // WINDOW_H_
