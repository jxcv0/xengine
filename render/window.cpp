#include "window.h"

#include <GLFW/glfw3.h>

#include <stdio.h>

#include "checkerr.h"
#include "lin.h"

/**
 * ----------------------------------------------------------------------------
 */
static void size_callback([[maybe_unused]] GLFWwindow *window, int width,
                          int height) {
  glViewport(0, 0, width, height);
}

/**
 * ----------------------------------------------------------------------------
 */
void create_window(GLFWwindow **window, const float width, const float height,
                   const char *name) {
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

  *window = glfwCreateWindow(width, height, name, nullptr, nullptr);

  if (*window == nullptr) {
    perror("window: unable to create window");
    glfwTerminate();
    return;
  }

  glfwMakeContextCurrent(*window);
  glfwSetFramebufferSizeCallback(*window, size_callback);

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
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr,
                          GL_TRUE);
  }

  glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // depth testing
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  // face culling
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
}

/**
 * ----------------------------------------------------------------------------
 */
Window::Window(float width, float height, const char *name)
    : m_width(width), m_height(height), mp_window(nullptr) {
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

  mp_window = glfwCreateWindow(m_width, m_height, name, nullptr, nullptr);

  if (mp_window == nullptr) {
    perror("window: unable to create window");
    glfwTerminate();
    return;
  }

  glfwMakeContextCurrent(mp_window);
  glfwSetFramebufferSizeCallback(
      mp_window, []([[maybe_unused]] GLFWwindow *window, int width,
                    int height) { glViewport(0, 0, width, height); });

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
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr,
                          GL_TRUE);
  }

  glfwSetInputMode(mp_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // depth testing
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  // face culling
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
}

/**
 * ----------------------------------------------------------------------------
 */
Window::~Window() {
  glfwDestroyWindow(mp_window);
  glfwTerminate();
}

/**
 * ----------------------------------------------------------------------------
 */
bool Window::should_close() { return glfwWindowShouldClose(mp_window); }

/**
 * ----------------------------------------------------------------------------
 */
void Window::swap_buffers() { glfwSwapBuffers(mp_window); }

/**
 * ----------------------------------------------------------------------------
 */
void Window::poll_events() { glfwPollEvents(); }

/**
 * ----------------------------------------------------------------------------
 */
void Window::set_bg(float r, float g, float b, float a) {
  glClearColor(r, g, b, a);
}

/**
 * ----------------------------------------------------------------------------
 */
void Window::clear_buffers() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/**
 * ----------------------------------------------------------------------------
 */
Mat4 Window::projection_matrix(const float fov) const noexcept {
  return lin::perspective(lin::radians(fov), ((float)m_width / (float)m_height),
                          0.1f, 100.0f);
}
