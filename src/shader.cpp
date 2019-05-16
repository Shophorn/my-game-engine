#include <string>
#include <GL/glew.h>
#include <iostream>

#include "shader.hpp"

namespace detail
{
	// Check if compilation of shader was succesful
	bool compileSuccessful(GLuint shaderId)
	{
		GLint status;
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);
		return status == GL_TRUE;
	}

	// If compilation was not succesful, get error messages
	std::string getErrorMessage(GLuint shaderId)
	{
		const int bufferLength = 1024;
		static char buffer [bufferLength];
		glGetShaderInfoLog(shaderId, bufferLength, nullptr, buffer);
		return std::string (buffer);
	}

	// Create and compile shader stage from source file.
	// shaderType must be GL_VERTEX_SHADER or GL_FRAGMENT_SHADER or similar
	auto createShader(const std::string & source, GLenum shaderType)
	{
		auto shaderId = glCreateShader(shaderType);
		auto sourcePtr = source.c_str();

		glShaderSource(shaderId, 1, &sourcePtr, nullptr);
		glCompileShader(shaderId);

		if (compileSuccessful(shaderId) == false)
		{
			std::cout << getErrorMessage(shaderId);
		}

		return shaderId;
	}
}

// Create complete linked shaderprogram from vertex and fragment sources
GLuint shader::fromSource(
	const std::string & vertexShaderSource,
	const std::string & fragmentShaderSource
){
	auto vertexShader = detail::createShader(vertexShaderSource, GL_VERTEX_SHADER);
	auto fragmentShader = detail::createShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glBindFragDataLocation(shaderProgram, 0, shader::outColorName);

	glLinkProgram(shaderProgram);

	return shaderProgram;
}