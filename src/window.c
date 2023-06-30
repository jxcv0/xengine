#include "window.h"

#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "checkerr.h"
#include "glad.h"

static void error_callback(int error, const char *desc) {
  fprintf(stderr, "GLFW (%d): %s\n", error, desc);
}

/**
 * ----------------------------------------------------------------------------
 */
static void size_callback(GLFWwindow *window, int width, int height) {
  (void)window;
  glViewport(0, 0, width, height);
}

/**
 * ----------------------------------------------------------------------------
 */
GLFWwindow *create_window(const float width, const float height,
                          const char *name) {
  glfwSetErrorCallback(error_callback);
  if (glfwInit() == GLFW_FALSE) {
    exit(EXIT_FAILURE);
  }
  int major, minor, rev;
  glfwGetVersion(&major, &minor, &rev);
  printf("GLFW Version: %d.%d.%d\n", major, minor, rev);
  printf("Using minimum context version 4.6\n");
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

  GLFWwindow *w = glfwCreateWindow(width, height, name, NULL, NULL);

  if (w == NULL) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(w);
  glfwSetFramebufferSizeCallback(w, size_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  int flags;
  glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
  if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(gl_debug_output, NULL);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL,
                          GL_TRUE);
  }

  // glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glEnable(GL_BLEND);

  return w;
}
