#ifndef SHADER_H
#define SHADER_H

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

#ifndef OPENGL_LIBS
#define OPENGL_LIBS
#include "glad.h"
#endif

#include <mat4.h>
#include <vec2.h>
#include <vec3.h>
#include <vec4.h>

/**
 * @brief A GLSL Shader.
 */
class Shader {
 public:
  /**
   * @brief Construct a shader from a program id.
   */
  explicit Shader(unsigned int id) : m_id(id){};

  /**
   * @brief Installs the program object as part of current rendering state.
   */
  inline void use() { glUseProgram(m_id); }

  /**
   * @brief Uniform boolean utility function.
   *
   * @param uniform_name The name of the uniform.
   * @param value The value to set the uniform to.
   */
  inline void set_uniform(const char *uniform_name, bool value) {
    glUniform1i(glGetUniformLocation(m_id, uniform_name),
                static_cast<int>(value));
  }

  /**
   * @brief Uniform integer utility function.
   *
   * @param uniform_name The name of the uniform.
   * @param value The value to set the uniform to.
   */
  inline void set_uniform(const char *uniform_name, const int value) {
    glUniform1i(glGetUniformLocation(m_id, uniform_name), value);
  }

  /**
   * @brief Uniform float utility function.
   *
   * @param uniform_name The name of the uniform.
   * @param value The value to set the uniform to.
   */
  inline void set_uniform(const char *uniform_name, const float value) {
    glUniform1f(glGetUniformLocation(m_id, uniform_name), value);
  }

  /**
   * @brief Uniform glm::vec2 float utility function.
   *
   * @param uniform_name The name of the uniform.
   * @param value The value to set the uniform to.
   */
  void set_uniform(const char *uniform_name, const Vec2 &value) {
    glUniform2fv(glGetUniformLocation(m_id, uniform_name), 1,
                 value.value_ptr());
  }

  /**
   * @brief Uniform glm::vec3 float utility function.
   *
   * @param uniform_name The name of the uniform.
   * @param value The value to set the uniform to.
   */
  void set_uniform(const char *uniform_name, const Vec3 &value) {
    glUniform3fv(glGetUniformLocation(m_id, uniform_name), 1,
                 value.value_ptr());
  }

  /**
   * @brief Uniform glm::vec4 float utility function.
   *
   * @param uniform_name The name of the uniform.
   * @param value The value to set the uniform to.
   */
  void set_uniform(const char *uniform_name, const Vec4 &value) {
    glUniform4fv(glGetUniformLocation(m_id, uniform_name), 1,
                 value.value_ptr());
  }

  /**
   * @brief Uniform glm::mat4 float utility function.
   *
   * @param uniform_name The name of the uniform.
   * @param value The value to set the uniform to.
   */
  void set_uniform(const char *uniform_name, const Mat4 &value) {
    auto loc = glGetUniformLocation(m_id, uniform_name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, value.value_ptr());
  }

 private:
  unsigned int m_id;
};

/**
 * @brief Utility functions.
 */
namespace ShaderUtils {

/** @brief Load a shader from a file.
 *  @param vert_path Path to the vertex shader.
 *  @param frag_path Path to the fragment shader.
 *  @return A new Shader object.
 */
Shader load(const std::filesystem::path &vert_path,
            const std::filesystem::path &frag_path);
}  // namespace ShaderUtils

#endif  // SHADER_H
