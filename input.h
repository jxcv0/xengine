#ifndef INPUT_H_
#define INPUT_H_

#include "GLFW/glfw3.h"
#include "lin.h"

struct mouse_pos {
  vec2_t m_curr_pos;
  vec2_t m_last_pos;
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
vec2_t get_cursor_offset(const struct mouse_pos *mp);

#endif  // INPUT_H_
