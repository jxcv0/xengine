#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <cstdint>

#include "mat4.h"
#include "vec2.h"

#ifndef GLAD_INCLUDED
#define GLAD_INCLUDED
#include <glad.h>
#endif
#include <GLFW/glfw3.h>

/**
 * @brief Wraps a GLFWWindow* in an raii class.
 */
class Window {
 public:
  /**
   * @brief Create and display a window with a width, height and name.
   *        Also initialize OpenGL context and debug logging.
   *
   * @param width The starting width of the window.
   * @param height The starting height of the window.
   * @param name The name of the window.
   */
  Window(float width, float height, const char *name);

  /**
   * @brief Destroy the window and terminate GLFW.
   */
  ~Window();

  /**
   * @brief  Check if the window should close.
   *
   * @return true if window should close, otherwise returns false.
   */
  bool should_close();

  /**
   * @brief Swap front and back buffer of the window.
   */
  void swap_buffers();

  /**
   * @brief Process all pending events.
   */
  void poll_events();

  /**
   * @brief Set the background color of the window.
   */
  void set_bg(float r, float g, float b, float a);

  /**
   * @brief Clear bit buffers.
   */
  void clear_buffers();

  /**
   * @brief Get the cursor position from the window.
   */
  Vec2 cursor_position() const noexcept;

  /**
   * @brief Calulate the projection matrix for the window.
   */
  Mat4 projection_matrix(const float fov) const noexcept;

  void update_input() {}

 private:
  float m_width;
  float m_height;
  GLFWwindow *mp_window;
  int m_input_buffer;
};

#endif  // MAINWINDOW_H_
