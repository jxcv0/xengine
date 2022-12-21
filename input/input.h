#ifndef INPUT_H_
#define INPUT_H_

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "vec2.h"
#include "window.h"

/**
 * @brief Get input from a window.
 */
class Input {
 public:
  /**
   * @brief Construct Input object.
   *
   * @param w Pointer to the window to get input from.
   */
  explicit Input(const Window *w) noexcept : mp_window(w->mp_window) {}

  /**
   * @brief Get the current cursor position.
   *
   * @return a Vec2 position.
   */
  Vec2 poll_cursor_pos() const noexcept;

  /**
   * @brief get the last state of a key.
   *
   * @param key The key.
   * @param state The state.
   * @return true if the last state of the key is the same as state, otherwise
   * false.
   */
  bool poll_key(const int key, const int state) const noexcept;

 private:
  GLFWwindow *mp_window;
};

#endif  // INPUT_H_
