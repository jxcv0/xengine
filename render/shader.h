#ifndef SHADER_H
#define SHADER_H

#include <stdbool.h>

#ifndef GLAD_INCLUDED
#define GLAD_INCLUDED
#include <glad.h>
#endif

#include "mat4.h"
#include "vec2.h"
#include "vec3.h"
#include "vec4.h"

using shader_t = unsigned int;

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
                            const Vec2 *value) {
  glUniform2fv(glGetUniformLocation(id, uniform_name), 1, value->value_ptr());
}

/**
 * @brief Uniform glm::vec3 float utility function.
 *
 * @param uniform_name The name of the uniform.
 * @param value The value to set the uniform to.
 */
void shader_set_uniform_3fv(shader_t id, const char *uniform_name,
                            const Vec3 *value) {
  glUniform3fv(glGetUniformLocation(id, uniform_name), 1, value->value_ptr());
}

/**
 * @brief Uniform glm::vec4 float utility function.
 *
 * @param uniform_name The name of the uniform.
 * @param value The value to set the uniform to.
 */
void set_uniform(shader_t id, const char *uniform_name, const Vec4 *value) {
  glUniform4fv(glGetUniformLocation(id, uniform_name), 1, value->value_ptr());
}

/**
 * @brief Uniform glm::mat4 float utility function.
 *
 * @param uniform_name The name of the uniform.
 * @param value The value to set the uniform to.
 */
void set_uniform(shader_t shader, const char *uniform_name, const Mat4 *value) {
  auto loc = glGetUniformLocation(shader, uniform_name);
  glUniformMatrix4fv(loc, 1, GL_FALSE, value->value_ptr());
}

/** @brief Load a shader from a file.
 *  @param vert_path Path to the vertex shader.
 *  @param frag_path Path to the fragment shader.
 *  @return A new Shader object.
 */
shader_t shader_load(const char *vert_path, const char *frag_path);

#endif  // SHADER_H
