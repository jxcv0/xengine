#include "shader.h"

#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

/**
 * ----------------------------------------------------------------------------
 */
void shader_set_uniform_1i(GLuint id, const char *uniform_name, int32_t value) {
  glUniform1i(glGetUniformLocation(id, uniform_name), value);
}

/**
 * ----------------------------------------------------------------------------
 */
void shader_set_uniform_1ui(GLuint id, const char *uniform_name,
                            uint32_t value) {
  glUniform1ui(glGetUniformLocation(id, uniform_name), value);
}

/**
 * ----------------------------------------------------------------------------
 */
void shader_set_uniform_1f(GLuint id, const char *uniform_name, float value) {
  glUniform1f(glGetUniformLocation(id, uniform_name), value);
}

/**
 * ----------------------------------------------------------------------------
 */
void shader_set_uniform_2fv(GLuint id, const char *uniform_name,
                            float value[2]) {
  glUniform2fv(glGetUniformLocation(id, uniform_name), 1, value);
}

/**
 * ----------------------------------------------------------------------------
 */
void shader_set_uniform_3fv(GLuint id, const char *uniform_name,
                            float value[3]) {
  glUniform3fv(glGetUniformLocation(id, uniform_name), 1, value);
}

/**
 * ----------------------------------------------------------------------------
 */
void shader_set_uniform_4fv(GLuint id, const char *uniform_name,
                            float value[4]) {
  glUniform4fv(glGetUniformLocation(id, uniform_name), 1, value);
}

/**
 * ----------------------------------------------------------------------------
 */
void shader_set_uniform_m4fv(GLuint shader, const char *uniform_name,
                             float value[4][4]) {
  int loc = glGetUniformLocation(shader, uniform_name);
  glUniformMatrix4fv(loc, 1, GL_FALSE, &value[0][0]);
}

/**
 * ----------------------------------------------------------------------------
 */
void check_compile(GLuint id) {
  GLint success;
  char infoLog[1024];
  glGetShaderiv(id, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(id, 1024, NULL, infoLog);
    printf("%s\n", infoLog);
    exit(EXIT_FAILURE);
  }
}

/**
 * ----------------------------------------------------------------------------
 */
void check_link(GLuint id) {
  GLint success;
  char infoLog[1024];
  glGetProgramiv(id, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(id, 1024, NULL, infoLog);
    printf("%s\n", infoLog);
    exit(EXIT_FAILURE);
  }
}

/**
 * ----------------------------------------------------------------------------
 */
GLuint load_shader_vf(const char *vert_path, const char *frag_path) {
  char *v = load_file_into_mem(vert_path);
  char *f = load_file_into_mem(frag_path);

  const char *vert_file = v;
  const char *frag_file = f;

  GLuint vert_id = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vert_id, 1, &vert_file, NULL);
  glCompileShader(vert_id);
  check_compile(vert_id);

  GLuint frag_id = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(frag_id, 1, &frag_file, NULL);
  glCompileShader(frag_id);
  check_compile(frag_id);

  GLuint program_id = glCreateProgram();
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

/**
 * ----------------------------------------------------------------------------
 */
GLuint load_shader_vfg(const char *vert_path, const char *frag_path,
                       const char *geom_path) {
  char *v = load_file_into_mem(vert_path);
  char *f = load_file_into_mem(frag_path);
  char *g = load_file_into_mem(geom_path);

  const char *vert_file = v;
  const char *frag_file = f;
  const char *geom_file = g;

  GLuint vert_id = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vert_id, 1, &vert_file, NULL);
  glCompileShader(vert_id);
  check_compile(vert_id);

  GLuint frag_id = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(frag_id, 1, &frag_file, NULL);
  glCompileShader(frag_id);
  check_compile(frag_id);

  GLuint geom_id = glCreateShader(GL_GEOMETRY_SHADER);
  glShaderSource(geom_id, 1, &geom_file, NULL);
  glCompileShader(geom_id);
  check_compile(geom_id);

  GLuint program_id = glCreateProgram();
  glAttachShader(program_id, vert_id);
  glAttachShader(program_id, frag_id);
  glAttachShader(program_id, geom_id);

  glLinkProgram(program_id);
  check_link(program_id);

  glDeleteShader(vert_id);
  glDeleteShader(frag_id);
  glDeleteShader(geom_id);

  free(v);
  free(f);
  free(g);

  return program_id;
}
