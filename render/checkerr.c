#include "checkerr.h"

#include <stdio.h>

void gl_debug_output(GLenum source, GLenum type, unsigned int id,
                     GLenum severity, GLsizei length, const char *message,
                     const void *user_param) {
  (void)length;
  (void)user_param;

  switch (source) {
    case GL_DEBUG_SOURCE_API:
      printf("api: ");
      break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
      printf("window system: ");
      break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
      printf("shader compiler: ");
      break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:
      printf("third party: ");
      break;
    case GL_DEBUG_SOURCE_APPLICATION:
      printf("application: ");
      break;
    case GL_DEBUG_SOURCE_OTHER:
      printf("other: ");
      break;
  }

  switch (type) {
    case GL_DEBUG_TYPE_ERROR:
      printf("error: ");
      break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
      printf("deprecated behaviour: ");
      break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
      printf("undefined behaviour: ");
      break;
    case GL_DEBUG_TYPE_PORTABILITY:
      printf("portability: ");
      break;
    case GL_DEBUG_TYPE_PERFORMANCE:
      printf("performance: ");
      break;
    case GL_DEBUG_TYPE_MARKER:
      printf("marker: ");
      break;
    case GL_DEBUG_TYPE_PUSH_GROUP:
      printf("push group: ");
      break;
    case GL_DEBUG_TYPE_POP_GROUP:
      printf("pop group: ");
      break;
    case GL_DEBUG_TYPE_OTHER:
      printf("other: ");
      break;
  }

  switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:
      printf("high severity: ");
      break;
    case GL_DEBUG_SEVERITY_MEDIUM:
      printf("medium severity: ");
      break;
    case GL_DEBUG_SEVERITY_LOW:
      printf("low severity: ");
      break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
      printf("notification: ");
      break;
  }

  printf("(%d): %s\n", id, message);
}
