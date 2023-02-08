#ifndef INPUT_H_
#define INPUT_H_

#include "GLFW/glfw3.h"
#include "lin.h"

#ifdef __cplusplus
extern "C" {
#endif

struct mouse_pos {
  vec2 m_curr_pos;
  vec2 m_last_pos;
};

/**
 * @brief Get the current cursor position.
 *
 * @param position The vec2 to store the mouse position in.
 * @param window The window to get the mouse position from.
 */
void get_cursor_position(struct mouse_pos *mp, GLFWwindow *window);

/**
 * @brief Calculate the change in cursor position and store the result in
 * offset.
 *
 * @param offset The vec2 to store the result in.
 * @param mp A pointer to the mouse position data to operate on.
 */
void get_cursor_offset(vec2 offset, const struct mouse_pos *mp);

#ifdef __cplusplus
}
#endif

#endif  // INPUT_H_
