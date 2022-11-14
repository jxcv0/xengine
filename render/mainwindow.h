#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <algorithm>
#include <cstdint>
#include <string>
#include <vector>

#ifndef OPENGL_LIBS
#define OPENGL_LIBS
#include <glad.h>
#endif

#include <GLFW/glfw3.h>
#include <lin.h>
// #include "keys.h"

class MainWindow {
 public:
  /*! \brief        Construct window with initial width and height.
   *  \param width  Initial witdth of the window.
   *  \param height Initial witdth of the window.
   *  \param name   The name of the window.
   */
  inline MainWindow(float width, float height, std::string name)
      : m_width(width), m_height(height), m_name(name), mp_window(nullptr) {
    glfwInit();
  }

  /*! \brief Destroy the window and terminate GLFW.
   */
  ~MainWindow() {
    glfwDestroyWindow(mp_window);
    glfwTerminate();
  }

  /*! \brief       Set GLFW window hints
   *  \param hint  The window hint enum
   *  \param value The new value of the hint
   */
  inline void set_hint(int hint, int value) const {
    glfwWindowHint(hint, value);
  }

  /*! \brief Create and show the window.
   */
  void show() {
    if (nullptr != mp_window) {
      return;
    }

    mp_window =
        glfwCreateWindow(m_width, m_height, m_name.data(), nullptr, nullptr);
    if (nullptr == mp_window) {
      perror("unable to create glfwwindow");
      glfwTerminate();
      return;
    }

    glfwMakeContextCurrent(mp_window);
    glfwSetFramebufferSizeCallback(
        mp_window, [](GLFWwindow *window, int width, int height) {
          glViewport(0, 0, width, height);
        });

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      perror("Unable to initialize GLAD");
      glfwTerminate();
      return;
    }

    glfwSetInputMode(mp_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // face culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
  }

  /*! \brief  Check if the window should close.
   *  \return true if window should close, otherwise returns false.
   */
  inline bool should_close() { return glfwWindowShouldClose(mp_window); }

  /*! \brief Swap front and back buffer of the window.
   */
  inline void swap_buffers() { glfwSwapBuffers(mp_window); }

  /*! \brief Process all pending events.
   */
  inline void poll_events() { glfwPollEvents(); }

  /*! \brief Set the background color of the window.
   */
  inline void set_bg(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
  }

  /*! \brief Clear bit buffers.
   */
  inline void clear_buffers() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  /*! \brief Set cursor position callback function.
   */
  template <typename Function>
  inline void set_cursor_position_callback(Function function) {
    glfwSetCursorPosCallback(mp_window, function);
  }

  // /*! \brief Update input buffer.
  //  */
  // void update_input() {
  //   m_input_buffer = 0;
  //   for (auto key : KEYS) {
  //     if (glfwGetKey(mp_window, key) == GLFW_PRESS) {
  //       m_input_buffer |= key;
  //     }
  //   }
  // }

  // /*! \brief     Linear search through active keys on this window
  //  *  \param key The key to check for.
  //  *  \return    true if key is pressed, otherwise returns false;
  //  */
  // bool key_pressed(int key) { return (m_input_buffer & key) != 0; }

  /*! \brief     Get a projection matrix for this window.
   *  \param fov The field of view.
   *  \return    The projection matrix.
   */
  auto projection_matrix(float fov) {
    if (fov != m_fov) {
      m_fov = fov;
      m_perspective_matrix = lin::perspective(
          m_fov, ((float)m_width / (float)m_height), 0.1f, 100.0f);
    }
    return m_perspective_matrix;
  }

 private:
  GLFWwindow *mp_window;
  float m_width = 640;
  float m_height = 480;
  std::string m_name = "XENGINE_WINDOW";
  std::uint32_t m_input_buffer;
  float m_fov = 0.0f;
  Mat4 m_perspective_matrix;
};

#endif  // MAINWINDOW_H_
