#ifndef INPUT_H_
#define INPUT_H_

#include "vec2.h"
#include "window.h"

/**
 * @brief Input handler component functions.
 */
typedef void (*input_handler_fp)(void);

  /**
   * @brief Get the current cursor position.
   *
   * @return a Vec2 position.
   */
Vec2 poll_cursor_pos(GLFWwindow *window);

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
  explicit Input(GLFWwindow *w) noexcept : mp_window(w) {}

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
   * @return The last state of the key
   */
  int poll_key(const int key) const;

 private:
  GLFWwindow *mp_window;
};

#endif  // INPUT_H_
