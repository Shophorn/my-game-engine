#version 330 core

in vec2 position;
in vec2 uv;

out vec2 UV;

void main ()
{
	UV = uv;
	gl_Position = vec4 (position, 0, 1);
}