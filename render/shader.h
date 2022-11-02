#ifndef SHADER_H
#define SHADER_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#ifndef OPENGL_LIBS
#define OPENGL_LIBS
#include <glad.h>
#endif

#include <glm/glm.hpp>

/**
 * @brief A GLSL Shader.
 */
class Shader {
 public:
  /**
   * @brief Construct a shader from a program id.
   */
  explicit Shader(unsigned int id) : m_id(id) {};

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
  void set_uniform(const char *uniform_name, const glm::vec2 &value) {
    glUniform2fv(glGetUniformLocation(m_id, uniform_name), 1, &value[0]);
  }

  /**
   * @brief Uniform glm::vec3 float utility function.
   *
   * @param uniform_name The name of the uniform.
   * @param value The value to set the uniform to.
   */
  void set_uniform(const char *uniform_name, const glm::vec3 &value) {
    glUniform3fv(glGetUniformLocation(m_id, uniform_name), 1, &value[0]);
  }

  /**
   * @brief Uniform glm::vec4 float utility function.
   *
   * @param uniform_name The name of the uniform.
   * @param value The value to set the uniform to.
   */
  void set_uniform(const char *uniform_name, const glm::vec4 &value) {
    glUniform4fv(glGetUniformLocation(m_id, uniform_name), 1, &value[0]);
  }

  /**
   * @brief Uniform glm::mat4 float utility function.
   *
   * @param uniform_name The name of the uniform.
   * @param value The value to set the uniform to.
   */
  void set_uniform(const char *uniform_name, const glm::mat4 &value) {
    auto loc = glGetUniformLocation(m_id, uniform_name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, &value[0][0]);
  }

 private:
  unsigned int m_id;  // Handle to the program object
};

namespace ShaderUtils {

/**
 * @brief Check the compilation status of a shader.
 *
 * @param id The id of the shader.
 */
void check_compile(int id);

/**
 * @brief Check the link status of the shader program.
 *
 * @throw ShaderLinkError
 */
void check_link(int id);

/** @brief Load a shader from a file.
 *  @param vert_path Path to the vertex shader.
 *  @param frag_path Path to the fragment shader.
 *  @return A new Shader object.
 *
 *  TODO shaders can all be loaded and compiled at once before game start.
 */
Shader load(const char *vert_path, const char *frag_path);
}  // namespace ShaderUtils

#endif  // SHADER_H
