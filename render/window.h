#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <cstdint>

#include "checkerr.h"

#ifndef OPENGL_LIBS
#define OPENGL_LIBS
#include <glad.h>
#endif
#include <GLFW/glfw3.h>

#include "lin.h"

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
  Window(float width, float height, const char *name)
      : m_width(width), m_height(height), mp_window(nullptr) {
    glfwInit();
    mp_window = glfwCreateWindow(m_width, m_height, name, nullptr, nullptr);

    if (mp_window == nullptr) {
      perror("window: unable to create window");
      glfwTerminate();
      return;
    }

    glfwMakeContextCurrent(mp_window);
    glfwSetFramebufferSizeCallback(
        mp_window, [](GLFWwindow *window, int width, int height) {
          glViewport(0, 0, width, height);
        });

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      perror("glad: unable to load opengl");
      glfwTerminate();
      return;
    }

    int flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
      glEnable(GL_DEBUG_OUTPUT);
      glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
      glDebugMessageCallback(gl_debug_output, nullptr);
      glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0,
                            nullptr, GL_TRUE);
    }

    glfwSetInputMode(mp_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // face culling
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);
  }

  /**
   * @brief Destroy the window and terminate GLFW.
   */
  ~Window() {
    glfwDestroyWindow(mp_window);
    glfwTerminate();
  }

  /**
   * @brief  Check if the window should close.
   *
   * @return true if window should close, otherwise returns false.
   */
  inline bool should_close() { return glfwWindowShouldClose(mp_window); }

  /**
   * @brief Swap front and back buffer of the window.
   */
  inline void swap_buffers() { glfwSwapBuffers(mp_window); }

  /**
   * @brief Process all pending events.
   */
  inline void poll_events() { glfwPollEvents(); }

  /**
   * @brief Set the background color of the window.
   */
  inline void set_bg(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
  }

  /**
   * @brief Clear bit buffers.
   */
  inline void clear_buffers() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  /**
   * @brief Set the cursor position callback.
   *
   * @param f The callback function
   */
  template <typename F>
  inline void set_cursor_position_callback(F f) {
    glfwSetCursorPosCallback(mp_window, f);
  }

  /**
   * @brief Calulate the projection matrix for the window.
   */
  auto projection_matrix(float fov) {
    return lin::perspective(lin::radians(fov),
                            ((float)m_width / (float)m_height), 0.1f, 100.0f);
  }

 private:
  GLFWwindow *mp_window;
  float m_width;
  float m_height;
  // std::uint32_t m_input_buffer;
};

#endif  // MAINWINDOW_H_
