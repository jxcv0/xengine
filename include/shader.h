#ifndef SHADER_H
#define SHADER_H

#include <glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace xen
{
	// check the compilation status of a shader program
	void checkShaderCompile(GLuint shaderId)
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
	void checkPrgmLink(GLuint prgmId)
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
	unsigned int loadShaderFromFile(const char *vertPath, const char* fragPath)
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
		checkShaderCompile(vertId);

		// frag
		fragId = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragId, 1, &fragCodeCstr, NULL);
		glCompileShader(fragId);
		checkShaderCompile(fragId);

		// prgm
		auto prgmId = glCreateProgram();
		glAttachShader(prgmId, vertId);
		glAttachShader(prgmId, fragId);
		glLinkProgram(prgmId);
		checkPrgmLink(prgmId);

		glDeleteShader(vertId);
		glDeleteShader(fragId);
		
		return prgmId;
	}

	// use a shader program
	void useShader(unsigned int shader)
	{
		glUseProgram(shader);
	}

	// set shader boolean uniform utility function
	void setShaderUniform(unsigned int shader, const char *uniformName, bool value)
	{
		glUniform1i(glGetUniformLocation(shader, uniformName), static_cast<int>(value));
	}

	// set shader int uniform utility function
	void setShaderUniform(unsigned int shader, const char *uniformName, int value)
	{
		glUniform1i(glGetUniformLocation(shader, uniformName), value);
	}

	// set shader float uniform utility function
	void setShaderUniform(unsigned int shader, const char *uniformName, float value)
	{
		glUniform1f(glGetUniformLocation(shader, uniformName), value);
	}

	// set shader vec2 uniform utility function
	void setShaderUniform(unsigned int shader, const char *uniformName, glm::vec2 &value)
	{
		glUniform2fv(glGetUniformLocation(shader, uniformName), 1, &value[0]);
	}

	// set shader vec3 uniform utility function
	void setShaderUniform(unsigned int shader, const char *uniformName, glm::vec3 &value)
	{
		glUniform3fv(glGetUniformLocation(shader, uniformName), 1, &value[0]);
	}

	// set shader vec4 uniform utility function
	void setShaderUniform(unsigned int shader, const char *uniformName, glm::vec4 &value)
	{
		glUniform4fv(glGetUniformLocation(shader, uniformName), 1, &value[0]);
	}

	// set shader mat4 uniform utility funtion
	void setShaderUniform(unsigned int shader, const char *uniformName, glm::mat4 &value)
	{
		glUniformMatrix4fv(glGetUniformLocation(shader, uniformName), 1, GL_FALSE, &value[0][0]);
	}
} // namespace xen

#endif // SHADER_H
