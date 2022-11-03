#include "shader.h"

/*------------------------------------------------------------------------------
 */
void ShaderUtils::check_compile(int id) {
  GLint success;
  char infoLog[1024];
  glGetShaderiv(id, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(id, 1024, NULL, infoLog);
    throw std::runtime_error(infoLog);
  }
}

/*------------------------------------------------------------------------------
 */
void ShaderUtils::check_link(int id) {
  GLint success;
  char infoLog[1024];
  glGetProgramiv(id, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(id, 1024, NULL, infoLog);
    throw std::runtime_error(infoLog);
  }
}

/*------------------------------------------------------------------------------
 */
Shader ShaderUtils::load(const char *vert_path, const char *frag_path) {
  std::string vert_code;
  std::string frag_code;
  std::ifstream vert_file;
  std::ifstream frag_file;

  vert_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  frag_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try {
    vert_file.open(vert_path);
    std::stringstream vertStream;
    vertStream << vert_file.rdbuf();
    vert_file.close();
    vert_code = vertStream.str();
  } catch (std::ifstream::failure &e) {
    std::cout << "Unable to load vertex shader " << vert_path << " " << e.what()
              << "\n";
  }

  try {
    frag_file.open(frag_path);
    std::stringstream fragStream;
    fragStream << frag_file.rdbuf();
    frag_file.close();
    frag_code = fragStream.str();
  } catch (std::ifstream::failure &e) {
    std::cout << "Unable to load fragment shader " << frag_path << " "
              << e.what() << "\n";
  }

  const char *vert_code_cstr = vert_code.c_str();
  const char *frag_code_cstr = frag_code.c_str();
  unsigned int vert_id, frag_id;

  vert_id = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vert_id, 1, &vert_code_cstr, NULL);
  glCompileShader(vert_id);
  try {
    check_compile(vert_id);
  } catch (const std::runtime_error &e) {
    std::cout << e.what();
  }

  frag_id = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(frag_id, 1, &frag_code_cstr, NULL);
  glCompileShader(frag_id);
  try {
    check_compile(frag_id);
  } catch (const std::runtime_error &e) {
    std::cout << e.what();
  }

  auto program_id = glCreateProgram();
  glAttachShader(program_id, vert_id);
  glAttachShader(program_id, frag_id);
  glLinkProgram(program_id);
  try {
    check_link(program_id);
  } catch (const std::runtime_error &e) {
    std::cout << e.what();
  }

  glDeleteShader(vert_id);
  glDeleteShader(frag_id);

  Shader shader(program_id);
  return shader;
}
