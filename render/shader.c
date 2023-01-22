#include "shader.h"

#include <stdio.h>
#include <stdlib.h>

#include "glad.h"
#include "utils.h"

/**
 * ----------------------------------------------------------------------------
 */
void shader_set_uniform_1i(shader_t id, const char *uniform_name,
                           const int value) {
  glUniform1i(glGetUniformLocation(id, uniform_name), value);
}

/**
 * ----------------------------------------------------------------------------
 */
void shader_set_uniform_1f(shader_t id, const char *uniform_name,
                           const float value) {
  glUniform1f(glGetUniformLocation(id, uniform_name), value);
}

/**
 * ----------------------------------------------------------------------------
 */
void shader_set_uniform_2fv(shader_t id, const char *uniform_name,
                            const vec2 value) {
  glUniform2fv(glGetUniformLocation(id, uniform_name), 1, value);
}

/**
 * ----------------------------------------------------------------------------
 */
void shader_set_uniform_3fv(shader_t id, const char *uniform_name,
                            const vec3 value) {
  glUniform3fv(glGetUniformLocation(id, uniform_name), 1, value);
}

/**
 * ----------------------------------------------------------------------------
 */
void shader_set_uniform_4fv(shader_t id, const char *uniform_name,
                            const vec4 value) {
  glUniform4fv(glGetUniformLocation(id, uniform_name), 1, value);
}

/**
 * ----------------------------------------------------------------------------
 */
void shader_set_uniform_m4fv(shader_t shader, const char *uniform_name,
                             const mat4 value) {
  int loc = glGetUniformLocation(shader, uniform_name);
  glUniformMatrix4fv(loc, 1, GL_FALSE, &value[0][0]);
}

/**
 * ----------------------------------------------------------------------------
 */
static void check_compile(int id) {
  GLint success;
  char infoLog[1024];
  glGetShaderiv(id, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(id, 1024, NULL, infoLog);
    printf("%s\n", infoLog);
  }
}

/**
 * ----------------------------------------------------------------------------
 */
static void check_link(int id) {
  GLint success;
  char infoLog[1024];
  glGetProgramiv(id, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(id, 1024, NULL, infoLog);
    printf("%s\n", infoLog);
  }
}

/**
 * ----------------------------------------------------------------------------
 */
shader_t load_shader(const char *vert_path, const char *frag_path) {
  char *v = load_file_into_mem(vert_path);
  char *f = load_file_into_mem(frag_path);

  const char *vert_file = v;
  const char *frag_file = f;

  unsigned int vert_id = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vert_id, 1, &vert_file, NULL);
  glCompileShader(vert_id);
  check_compile(vert_id);

  unsigned int frag_id = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(frag_id, 1, &frag_file, NULL);
  glCompileShader(frag_id);
  check_compile(frag_id);

  unsigned int program_id = glCreateProgram();
  glAttachShader(program_id, vert_id);
  glAttachShader(program_id, frag_id);
  glLinkProgram(program_id);

  check_link(program_id);

  glDeleteShader(vert_id);
  glDeleteShader(frag_id);

  free(v);
  free(f);

  return program_id;
}
