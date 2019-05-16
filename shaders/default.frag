#version 330 core

in vec2 UV;

out vec4 outColor;


void main()
{
	float b = 0.5;
	outColor = vec4(UV.x, UV.y, b, 1);
}