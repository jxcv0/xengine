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
void create_window(GLFWwindow **window, const float width, const float height,
                   const char *name) {
  glfwInit();
  glfwSetErrorCallback(error_callback);
  int major, minor, rev;
  glfwGetVersion(&major, &minor, &rev);
  printf("GLFW Version: %d.%d.%d\n", major, minor, rev);
  printf("Using minimum context version 4.6\n");
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

  *window = glfwCreateWindow(width, height, name, NULL, NULL);

  if (*window == NULL) {
    perror("Unable to create window");
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(*window);
  glfwSetFramebufferSizeCallback(*window, size_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    perror("Unable to load OpenGL");
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

  glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glEnable(GL_BLEND);
}

/**
 * ----------------------------------------------------------------------------
 */
void create_editor_window(GLFWwindow **window, const float width,
                          const float height, const char *name) {
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
  glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
  // glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  *window = glfwCreateWindow(width, height, name, NULL, NULL);

  if (*window == NULL) {
    perror("window: unable to create window");
    glfwTerminate();
    return;
  }

  glfwMakeContextCurrent(*window);

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
    glDebugMessageCallback(gl_debug_output, NULL);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL,
                          GL_TRUE);
  }

  glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}