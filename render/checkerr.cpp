#include "checkerr.h"

#include <ostream>
#include <string>

int gl_print_error(std::ostream &os, const char *file, int line) {
  GLenum errorCode;
  while ((errorCode = glGetError()) != GL_NO_ERROR) {
    std::string error;
    switch (errorCode) {
      case GL_INVALID_ENUM:
        error = "INVALID_ENUM";
        break;
      case GL_INVALID_VALUE:
        error = "INVALID_VALUE";
        break;
      case GL_INVALID_OPERATION:
        error = "INVALID_OPERATION";
        break;
      case GL_STACK_OVERFLOW:
        error = "STACK_OVERFLOW";
        break;
      case GL_STACK_UNDERFLOW:
        error = "STACK_UNDERFLOW";
        break;
      case GL_OUT_OF_MEMORY:
        error = "OUT_OF_MEMORY";
        break;
      case GL_INVALID_FRAMEBUFFER_OPERATION:
        error = "INVALID_FRAMEBUFFER_OPERATION";
        break;
    }
    os << error << " | " << file << " (" << line << ")"
       << "\n";
  }
  return errorCode;
}

void gl_debug_output(GLenum source, GLenum type, unsigned int id,
                     GLenum severity, [[maybe_unused]] GLsizei length,
                     const char *message,
                     [[maybe_unused]] const void *user_param) {
  switch (source) {
    case GL_DEBUG_SOURCE_API:
      std::cout << "api: ";
      break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
      std::cout << "window system: ";
      break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
      std::cout << "shader compiler: ";
      break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:
      std::cout << "third party: ";
      break;
    case GL_DEBUG_SOURCE_APPLICATION:
      std::cout << "application: ";
      break;
    case GL_DEBUG_SOURCE_OTHER:
      std::cout << "other: ";
      break;
  }

  switch (type) {
    case GL_DEBUG_TYPE_ERROR:
      std::cout << "error: ";
      break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
      std::cout << "deprecated behaviour: ";
      break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
      std::cout << "undefined behaviour: ";
      break;
    case GL_DEBUG_TYPE_PORTABILITY:
      std::cout << "portability: ";
      break;
    case GL_DEBUG_TYPE_PERFORMANCE:
      std::cout << "performance: ";
      break;
    case GL_DEBUG_TYPE_MARKER:
      std::cout << "marker: ";
      break;
    case GL_DEBUG_TYPE_PUSH_GROUP:
      std::cout << "push group: ";
      break;
    case GL_DEBUG_TYPE_POP_GROUP:
      std::cout << "pop group: ";
      break;
    case GL_DEBUG_TYPE_OTHER:
      std::cout << "other: ";
      break;
  }

  switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:
      std::cout << "high severity: ";
      break;
    case GL_DEBUG_SEVERITY_MEDIUM:
      std::cout << "medium severity";
      break;
    case GL_DEBUG_SEVERITY_LOW:
      std::cout << "low severity";
      break;
      // case GL_DEBUG_SEVERITY_NOTIFICATION:
      // std::cout << "";
      // break;
  }

  std::cout << "(" << id << "): " << message;
  std::cout << "\n";
}