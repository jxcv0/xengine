#ifndef SHADER_H
#define SHADER_H

#include <glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "math.h"
#include "shaderexceptions.h"

/*! \brief A GLSL Shader.
 */
class Shader {
    uint32_t m_id; // Handle to the program object

public:

    Shader() { }

    /*! \brief Installs the program object as part of current rendering state.
     */
	inline void use() { glUseProgram(id); }

    /*! \brief Check the compilation status of the shader program.
     */
	void check_compile() {
		GLint success;
		GLchar infoLog[1024];
		glGetShaderiv(m_id, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(m_id, 1024, NULL, infoLog);
            throw ShaderCompileException(infoLog);
		}
	}

	void check_link() {
		GLint success;
		GLchar infoLog[1024];
		glGetProgramiv(m_id, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(m_id, 1024, NULL, infoLog);
            throw ShaderLinkException(infoLog);
		}
	}

	// set shader boolean uniform utility function
	void set_uniform(unsigned int shader, const char *uniformName, bool value)
	{
        auto loc = glGetUniformLocation(shader, uniformName);
		glUniform1i(loc, static_cast<int>(value));
	}

	// set shader int uniform utility function
	void set_uniform(unsigned int shader, const char *uniformName, const int value)
	{
		glUniform1i(glGetUniformLocation(shader, uniformName), value);
	}

	// set shader float uniform utility function
	void set_uniform(unsigned int shader, const char *uniformName, const float value)
	{
		glUniform1f(glGetUniformLocation(shader, uniformName), value);
	}

	// set shader vec2 uniform utility function
	void set_uniform(unsigned int shader, const char *uniformName, const glm::vec2 &value)
	{
		glUniform2fv(glGetUniformLocation(shader, uniformName), 1, &value[0]);
	}

	// set shader vec3 uniform utility function
	void set_uniform(unsigned int shader, const char *uniformName, const glm::vec3 &value)
	{
		glUniform3fv(glGetUniformLocation(shader, uniformName), 1, &value[0]);
	}

	// set shader vec4 uniform utility function
	void set_uniform(unsigned int shader, const char *uniformName, const glm::vec4 &value)
	{
		glUniform4fv(glGetUniformLocation(shader, uniformName), 1, &value[0]);
	}

	// set shader mat4 uniform utility funtion
	void set_uniform(unsigned int shader, const char *uniformName, const glm::mat4 &value)
	{
		glUniformMatrix4fv(glGetUniformLocation(shader, uniformName), 1, GL_FALSE, &value[0][0]);
	}
}

namespace shader
{
	// check the compilation status of a shader program

	// check the linking status of a shader program

	// load a shader from a text file
	Shader load_shader(const char *vertPath, const char* fragPath)
	{
		std::string vertCode;
		std::string fragCode;
		std::ifstream vertFile;
		std::ifstream fragFile;
		vertFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fragFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{ vertFile.open(vertPath);
			fragFile.open(fragPath);
			std::stringstream vertStream, fragStream;
			vertStream << vertFile.rdbuf();
			fragStream << fragFile.rdbuf();
			vertFile.close();
			fragFile.close();
			vertCode = vertStream.str();
			fragCode = fragStream.str();
		}
		catch (std::ifstream::failure &e) {
			std::cout << "ERROR::SHADER::FILE_NOT_READ: " << e.what() << "\n";
		}
		
		const char *vertCodeCstr = vertCode.c_str();
		const char *fragCodeCstr = fragCode.c_str();
		unsigned int vertId, fragId;

		// vert
		vertId = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertId, 1, &vertCodeCstr, NULL);
		glCompileShader(vertId);
		check_compile(vertId);

		// frag
		fragId = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragId, 1, &fragCodeCstr, NULL);
		glCompileShader(fragId);
		check_compile(fragId);

		// prgm
		auto prgmId = glCreateProgram();
		glAttachShader(prgmId, vertId);
		glAttachShader(prgmId, fragId);
		glLinkProgram(prgmId);
		check_link(prgmId);

		glDeleteShader(vertId);
		glDeleteShader(fragId);
		
		return prgmId;
	}



/* Internal math lib is WIP
    // set shader Vec<2> uniform utility function
    void set_uniform(unsigned int shader, const char* uniformName, Vec<2>& value)
    {
		glUniform2fv(glGetUniformLocation(shader, uniformName), 1, &value[0]);
    }
    //
    // set shader Vec<2> uniform utility function
    void set_uniform(unsigned int shader, const char* uniformName, Vec<3>& value)
    {
		glUniform3fv(glGetUniformLocation(shader, uniformName), 1, &value[0]);
    }

    // set shader Mat<4, 4> uniform utility function
    void set_uniform(unsigned int shader, const char* uniformName, Mat<4, 4>&& value)
    {
		glUniformMatrix4fv(glGetUniformLocation(shader, uniformName), 1, GL_FALSE, &value[0][0]);
    }
*/
} // namespace xen

#endif // SHADER_H
