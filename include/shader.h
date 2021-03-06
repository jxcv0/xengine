#ifndef SHADER_H
#define SHADER_H

#include <glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "math.h"

namespace xen::shader
{
	// check the compilation status of a shader program
	void check_compile(GLuint shaderId)
	{
		GLint success;
		GLchar infoLog[1024];
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shaderId, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERR: " << infoLog << "\n";
		}
	}

	// check the linking status of a shader program
	void check_link(GLuint prgmId)
	{
		GLint success;
		GLchar infoLog[1024];
		glGetProgramiv(prgmId, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(prgmId, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERR: " << infoLog << "\n";
		}
	}

	// load a shader from a text file
	unsigned int load_shader(const char *vertPath, const char* fragPath)
	{
		std::string vertCode;
		std::string fragCode;
		std::ifstream vertFile;
		std::ifstream fragFile;
		vertFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fragFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			vertFile.open(vertPath);
			fragFile.open(fragPath);
			std::stringstream vertStream, fragStream;
			vertStream << vertFile.rdbuf();
			fragStream << fragFile.rdbuf();
			vertFile.close();
			fragFile.close();
			vertCode = vertStream.str();
			fragCode = fragStream.str();
		}
		catch (std::ifstream::failure &e)
		{
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

	// use a shader program
	void use_shader(unsigned int shader)
	{
		glUseProgram(shader);
	}

	// set shader boolean uniform utility function
	void set_uniform(unsigned int shader, const char *uniformName, bool value)
	{
		glUniform1i(glGetUniformLocation(shader, uniformName), static_cast<int>(value));
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

    // own math lib


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
} // namespace xen

#endif // SHADER_H
