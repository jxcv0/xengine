#include "input.h"

/**
 * ----------------------------------------------------------------------------
 */
void get_cursor_position(struct mouse_pos *mp, GLFWwindow *window) {
  // copy curr into last
  mp->m_last_pos.x = mp->m_curr_pos.x;
  mp->m_last_pos.y = mp->m_curr_pos.y;

  // set curr to new pos
  double x_temp, y_temp;
  glfwGetCursorPos(window, &x_temp, &y_temp);
  mp->m_curr_pos.x = (float)(x_temp);
  mp->m_curr_pos.y = (float)(y_temp);
}

/**
 * ----------------------------------------------------------------------------
 */
vec2_t get_cursor_offset(const struct mouse_pos *mp) {
    vec2_t offset = { .elem = {
  mp->m_curr_pos.x - mp->m_last_pos.x,
  mp->m_last_pos.y - mp->m_curr_pos.y
    }};
    return offset;
}
