#include "input.h"

#include "vec2.h"

bool Input::poll_key(const int key, const int state) const noexcept {
  return (glfwGetKey(mp_window, key) == state);
};

Vec2 Input::poll_cursor_pos() const noexcept {
  double x_temp, y_temp;
  glfwGetCursorPos(mp_window, &x_temp, &y_temp);
  float x = static_cast<float>(x_temp);
  float y = static_cast<float>(y_temp);
  return Vec2(x, y);
}
