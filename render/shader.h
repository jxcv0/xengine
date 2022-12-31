#ifndef SHADER_H
#define SHADER_H

#include <stdbool.h>

#include "lin.h"

#ifndef GLAD_INCLUDED
#define GLAD_INCLUDED
#include <glad.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned int shader_t;

/**
 * @brief Uniform integer utility function.
 *
 * @param uniform_name The name of the uniform.
 * @param value The value to set the uniform to.
 */
void shader_set_uniform_1i(shader_t id, const char *uniform_name,
                           const int value) {
  glUniform1i(glGetUniformLocation(id, uniform_name), value);
}

/**
 * @brief Uniform float utility function.
 *
 * @param uniform_name The name of the uniform.
 * @param value The value to set the uniform to.
 */
void shader_set_uniform_1f(shader_t id, const char *uniform_name,
                           const float value) {
  glUniform1f(glGetUniformLocation(id, uniform_name), value);
}

/**
 * @brief Uniform glm::vec2 float utility function.
 *
 * @param uniform_name The name of the uniform.
 * @param value The value to set the uniform to.
 */
void shader_set_uniform_2fv(shader_t id, const char *uniform_name,
                            const vec2 value) {
  glUniform2fv(glGetUniformLocation(id, uniform_name), 1, value);
}

/**
 * @brief Uniform glm::vec3 float utility function.
 *
 * @param uniform_name The name of the uniform.
 * @param value The value to set the uniform to.
 */
void shader_set_uniform_3fv(shader_t id, const char *uniform_name,
                            const vec3 value) {
  glUniform3fv(glGetUniformLocation(id, uniform_name), 1, value);
}

/**
 * @brief Uniform glm::vec4 float utility function.
 *
 * @param uniform_name The name of the uniform.
 * @param value The value to set the uniform to.
 */
void shader_set_uniform_4fv(shader_t id, const char *uniform_name, const vec4 value) {
  glUniform4fv(glGetUniformLocation(id, uniform_name), 1, value);
}

/**
 * @brief Uniform glm::mat4 float utility function.
 *
 * @param uniform_name The name of the uniform.
 * @param value The value to set the uniform to.
 */
void shader_set_uniform_m4fv(shader_t shader, const char *uniform_name, const mat4 value) {
  auto loc = glGetUniformLocation(shader, uniform_name);
  glUniformMatrix4fv(loc, 1, GL_FALSE, &value[0][0]);
}

/** @brief Load a shader from a file.
 *  @param vert_path Path to the vertex shader.
 *  @param frag_path Path to the fragment shader.
 *  @return A new Shader object.
 */
shader_t shader_load(const char *vert_path, const char *frag_path);

#ifdef __cplusplus
}
#endif

#endif  // SHADER_H
