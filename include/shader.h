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
		fragId = glCreateShader(GL_VERTEX_SHADER);
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
}

#endif // SHADER_H
