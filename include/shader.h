#ifndef SHADER_H
#define SHADER_H

#include <cstdint>
#include <stdexcept>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#ifndef OPENGL_LIBS
#define OPENGL_LIBS
#include <glad.h>
#endif

#include <glm/glm.hpp>

/*! \brief A GLSL Shader.
 */
class Shader {
  uint32_t m_id; // Handle to the program object

public:

  Shader() = delete;

  /*! \brief No default generated constructor.
   */
  Shader(uint32_t id) : m_id(id) {};

  /*! \brief Installs the program object as part of current rendering state.
   */
	inline void use() { glUseProgram(m_id); }

	// set shader boolean uniform utility function
	inline void set_uniform(unsigned int shader,
                          const char *uniformName,
                          bool value) {
		glUniform1i(glGetUniformLocation(shader, uniformName),
                static_cast<int>(value));
	}

	// set shader int uniform utility function
	inline void set_uniform(unsigned int shader,
                          const char *uniformName,
                          const int value) {
		glUniform1i(glGetUniformLocation(shader, uniformName), value);
	}

	// set shader float uniform utility function
	inline void set_uniform(unsigned int shader,
                          const char *uniformName,
                          const float value) {
		glUniform1f(glGetUniformLocation(shader, uniformName), value);
	}

	// set shader vec2 uniform utility function
	void set_uniform(unsigned int shader,
                   const char *uniformName,
                   const glm::vec2 &value) {
		glUniform2fv(glGetUniformLocation(shader, uniformName), 1, &value[0]);
	}

	// set shader vec3 uniform utility function
	void set_uniform(unsigned int shader,
                   const char *uniformName,
                   const glm::vec3 &value) {
		glUniform3fv(glGetUniformLocation(shader, uniformName), 1, &value[0]);
	}

	// set shader vec4 uniform utility function
	void set_uniform(unsigned int shader,
                   const char *uniformName,
                   const glm::vec4 &value) {
		glUniform4fv(glGetUniformLocation(shader, uniformName), 1, &value[0]);
	}

	// set shader mat4 uniform utility funtion
	void set_uniform(unsigned int shader,
                   const char *uniformName,
                   const glm::mat4 &value) {
    auto loc = glGetUniformLocation(shader, uniformName);
		glUniformMatrix4fv(loc, 1, GL_FALSE, &value[0][0]);
	}
};

namespace ShaderUtils {

  /*! \brief Check the compilation status of the shader program.
   *  \throw ShaderCompilerError
   */
	void check_compile(int id);

  /*! \brief Check the link status of the shader program.
   *  \throw ShaderLinkError
   */
	void check_link(int id);

  /*! \brief           Load a shader from a file.
   *  \param vert_path Path to the vertex shader.
   *  \param frag_path Path to the fragment shader.
   *  \return          A new Shader object.
   *
   *  TODO shaders can all be loaded and compiled at once before game start.
   */
	Shader load(const char *vert_path, const char *frag_path);
} // namespace Shader

#endif // SHADER_H

