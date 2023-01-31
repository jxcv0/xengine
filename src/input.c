#include "input.h"

/**
 * ----------------------------------------------------------------------------
 */
void get_cursor_position(struct mouse_pos *mp, GLFWwindow *window) {

  // copy curr into last
  mp->m_last_pos[0] = mp->m_curr_pos[0];
  mp->m_last_pos[1] = mp->m_curr_pos[1];

  // set curr to new pos
  double x_temp, y_temp;
  glfwGetCursorPos(window, &x_temp, &y_temp);
  mp->m_curr_pos[0] = (float)(x_temp);
  mp->m_curr_pos[1] = (float)(y_temp);
}

/**
 * ----------------------------------------------------------------------------
 */
void get_cursor_offset(vec2 offset, const struct mouse_pos *mp) {
  offset[0] = mp->m_curr_pos[0] - mp->m_last_pos[0];
  offset[1] = mp->m_last_pos[1] - mp->m_curr_pos[1];
}
