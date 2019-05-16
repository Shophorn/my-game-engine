#pragma once

#include <string>
#include <GL/glew.h>

namespace shader
{
	constexpr const char * outColorName = "outColor";
	constexpr const char * positionName = "position";

	// Create complete linked shaderprogram from vertex and fragment sources
	GLuint fromSource (
		const std::string & vertexShaderSource,
		const std::string & fragmentShaderSource
	);
}