#include "shader.h"

#include <exception>
#include <fstream>
#include <sstream>

static inline void check_compile(int id) {
  GLint success;
  char infoLog[1024];
  glGetShaderiv(id, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(id, 1024, NULL, infoLog);
    throw std::runtime_error(infoLog);
  }
}

static inline void check_link(int id) {
  GLint success;
  char infoLog[1024];
  glGetProgramiv(id, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(id, 1024, NULL, infoLog);
    throw std::runtime_error(infoLog);
  }
}

Shader ShaderUtils::load(const std::filesystem::path &vert_path,
                         const std::filesystem::path &frag_path) {
  std::ifstream vert_stream(vert_path);
  std::ifstream frag_stream(frag_path);

  std::stringstream vert_ss;
  std::stringstream frag_ss;

  vert_ss << vert_stream.rdbuf();
  frag_ss << frag_stream.rdbuf();

  auto vert = vert_ss.str();
  auto frag = frag_ss.str();

  const char *vert_code_cstr = vert.c_str();
  const char *frag_code_cstr = frag.c_str();

  unsigned int vert_id;
  unsigned int frag_id;

  vert_id = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vert_id, 1, &vert_code_cstr, nullptr);
  glCompileShader(vert_id);

  check_compile(vert_id);

  frag_id = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(frag_id, 1, &frag_code_cstr, nullptr);
  glCompileShader(frag_id);

  check_compile(frag_id);

  auto program_id = glCreateProgram();
  glAttachShader(program_id, vert_id);
  glAttachShader(program_id, frag_id);
  glLinkProgram(program_id);

  check_link(program_id);

  glDeleteShader(vert_id);
  glDeleteShader(frag_id);

  Shader shader(program_id);
  return shader;
}
