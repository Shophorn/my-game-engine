// #include <iostream>
#include <string>

// #define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "file.hpp"
#include "shader.hpp"

#include "maths/vectors.hpp"
#include "maths/matrices.hpp"

#include "debug.hpp"
#include "MeshData.hpp"

#include <fmt/format.h>

#include <thread>
#include <chrono>

std::string gameTitle = "Boss Game";
std::string vertexShaderPath = "shaders/screen_space.vert";
std::string fragmentShaderPath = "shaders/mandelbrot.frag";
// std::string fragmentShaderPath = "shaders/default.frag";

float2 gVertices [] =
{
	// Upper triangle
	float2 {-1, -1},
	float2 { 1, -1},
	float2 {-1,  1},

	// Lower triangle
	float2 {-1,  1},
	float2 { 1, -1},
	float2 { 1,  1},
};

float2 gUvs [] =
{
	float2 {0, 0},
	float2 {1, 0},
	float2 {0, 1},

	float2 {0, 1},
	float2 {1, 0},
	float2 {1, 1},
};

double distortSpeed = 0.1;
double moveSpeed = 0.7;
double zoomSpeed = 0.5;
double scale = 1;

double2 offset;
double2 constant;

double lastTime = 0;
double deltaTime;

void reset()
{
	scale = 1;
	offset = double2 (0, 0);
	constant = double2 (0, 0);
}

GLint styleLocation;

// ng is engine namespace
using namespace ng;

template <typename T>
constexpr bool testQuality = std::is_standard_layout_v<T> || std::is_pod_v<T>;

int main()
{

	// testVectors();
	// testMatrices();
	testVectors2();

	return 0;
	
	// for timer
	glfwInit();

	debug::initialize("c:/users/leo/desktop/logsDirectory/");
	debug::log("this seems good enough");

	std::vector<float2> mVertices 	{ gVertices, gVertices + 6 };
	std::vector<float2> mUvs 		{ gUvs, gUvs + 6 };
	// MeshData a;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow * window = glfwCreateWindow(800, 600, gameTitle.c_str(), nullptr, nullptr);

	glfwMakeContextCurrent(window);

	glewExperimental = true;
	glewInit();

	GLuint shaderId = shader::fromSource(
		file::readText(vertexShaderPath),
		file::readText(fragmentShaderPath)
	);
	glUseProgram(shaderId);
	glUniform3f(glGetUniformLocation(shaderId, "resolution"), 800, 600, 800.0 / 600.0);

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(gVertices),
		gVertices[0].valuePtr(),
		GL_STATIC_DRAW
	);

	GLint positionAttribute = glGetAttribLocation(shaderId, shader::positionName);
	glVertexAttribPointer (positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(positionAttribute);

	GLuint tbo;
	glGenBuffers(1, &tbo);
	glBindBuffer(GL_ARRAY_BUFFER, tbo);
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(gUvs), 
		gUvs[0].valuePtr(),
		GL_STATIC_DRAW
	);

	GLint uvAttribute = glGetAttribLocation(shaderId, "uv");
	glVertexAttribPointer(uvAttribute, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(uvAttribute);

	styleLocation = glGetUniformLocation(shaderId, "set");

	auto controls  = [] (GLFWwindow * window,  int key, int scancode, int action, int mods)
	{
		if (action == GLFW_PRESS || action == GLFW_REPEAT)
		{
			// Ctrl, zoom
			if ((mods & GLFW_MOD_CONTROL) > 0)
			{
				double step = deltaTime * zoomSpeed * scale;

				switch(key)
				{
					case GLFW_KEY_DOWN: scale += step;	break;
					case GLFW_KEY_UP: 	scale -= step;	break;

					// constants defined in shader
					case GLFW_KEY_J: 	glUniform1i(styleLocation, 0); break;
					case GLFW_KEY_M: 	glUniform1i(styleLocation, 1); break;

					case GLFW_KEY_R:	reset();		break;
				}	
			}

			// Shift, distort
			else if((mods & GLFW_MOD_SHIFT) > 0)
			{
				double step = deltaTime * distortSpeed * scale;

				switch(key)
				{
					case GLFW_KEY_LEFT: 	constant.x -= step; break;
					case GLFW_KEY_RIGHT:	constant.x += step; break;
					case GLFW_KEY_DOWN: 	constant.y -= step; break;
					case GLFW_KEY_UP: 		constant.y += step; break;
				}

			}

			// No mod, move
			else
			{
				double step = deltaTime * moveSpeed * scale;

				switch(key)
				{
					case GLFW_KEY_LEFT: 	offset.x -= step; 	break;
					case GLFW_KEY_RIGHT:	offset.x += step;	break;
					case GLFW_KEY_DOWN: 	offset.y -= step;	break;
					case GLFW_KEY_UP: 		offset.y += step;	break;
				}
			}
		}
	};

	glfwSetKeyCallback(window, controls);

	GLint offsetLocation = glGetUniformLocation(shaderId, "offset");
	GLint scaleLocation = glGetUniformLocation(shaderId, "scale");
	GLint constLocation = glGetUniformLocation(shaderId, "constant");

	// glViewport(0, 0, 400, 300);

	while(glfwWindowShouldClose(window) == false)
	{
			
		float time = glfwGetTime();
		deltaTime = time - lastTime;
		lastTime = time;

		glUniform2d(offsetLocation, offset.x, offset.y);
		glUniform2d(constLocation, constant.x, constant.y);
		glUniform1d(scaleLocation, scale);

		glDrawArrays(GL_TRIANGLES, 0, 6);
		
		glfwSwapBuffers(window);
	
		glfwPollEvents();

		glFinish();
	}

	glfwTerminate();
}