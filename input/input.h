#ifndef INPUT_H_
#define INPUT_H_

#include "GLFW/glfw3.h"
#include "lin.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Input handler component functions.
 */
typedef void (*input_handler_fp)(void);

/**
 * @brief Get the current cursor position.
 *
 * @return a Vec2 position.
 */
void input_poll_cursor_pos(vec2 pos, GLFWwindow *window);

#ifdef __cplusplus
}
#endif

#endif  // INPUT_H_
