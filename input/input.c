#include "input.h"

void poll_cursor_pos(vec2 vec, GLFWwindow *window) {
  double x_temp, y_temp;
  glfwGetCursorPos(window, &x_temp, &y_temp);
  vec[0] = (float)(x_temp);
  vec[1] = (float)(y_temp);
}
