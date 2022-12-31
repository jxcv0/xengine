#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#ifndef GLAD_INCLUDED
#define GLAD_INCLUDED
#include <glad.h>
#endif
#include <GLFW/glfw3.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Create window and correct OpenGl context.
 *
 * @param window A pointer to a GLFWwindow.
 */
void create_window(GLFWwindow **window, const float width, const float height,
                   const char *name);

#ifdef __cplusplus
}
#endif

#endif  // MAINWINDOW_H_
