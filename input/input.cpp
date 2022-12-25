#include "input.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "vec2.h"

Vec2 poll_cursor_pos(GLFWwindow *window) {
  double x_temp, y_temp;
  glfwGetCursorPos(window, &x_temp, &y_temp);
  float x = static_cast<float>(x_temp);
  float y = static_cast<float>(y_temp);
  return Vec2(x, y);
}

int Input::poll_key(const int key) const { return glfwGetKey(mp_window, key); };

Vec2 Input::poll_cursor_pos() const noexcept {
  double x_temp, y_temp;
  glfwGetCursorPos(mp_window, &x_temp, &y_temp);
  float x = static_cast<float>(x_temp);
  float y = static_cast<float>(y_temp);
  return Vec2(x, y);
}
