#version 400 core

in vec2 UV;

out vec4 outColor;

uniform vec3 resolution;

uniform double scale = 0.1;
uniform dvec2 offset = dvec2(0.3, 0.0);
uniform dvec2 constant = dvec2(0.3, 0.3);

const int colorArrayLength = 7;
const int lastColorIndex = colorArrayLength - 1;
uniform vec4 colors [colorArrayLength] = {
	vec4(1, 1, 1, 1),
	vec4(0, 0, 0, 1),
	vec4(0.1, 0.9,0.9, 1),
	vec4(0.7, 0.2, 0.35, 1),
	vec4(0.6, 0.2, 0.9, 1),
	vec4(0.8, 0.9, 0.12, 1),
	vec4(1, 1, 1, 1),
};

const int JULIA_SET = 0;
const int MANDELBROT_SET = 1;

// using julá set or mandelbrot set
uniform int set;

// Multiply complex with itself. 
dvec2 complexSquared(dvec2 c)
{
	return dvec2 (c.x * c.x - c.y * c.y, 2 * c.x * c.y);
}

double sqrLength(dvec2 vec)
{
	return vec.x * vec.x + vec.y * vec.y;
}


void main()
{
	dvec2 pos = (gl_FragCoord.xy / resolution.xy) * 2 - 1;
	pos *= scale;
	pos.x *= resolution.z;
	pos += offset;

	dvec2 _sample, _const;
	if (set == JULIA_SET)
	{
		_sample = pos;
		_const = constant;
	}
	else if (set == MANDELBROT_SET)
	{
		_sample = dvec2(0.0, 0.0); // constant at 0
		_const = pos;
	}

	// Do iteration
	float it = 0;
	float maxIt = 500;

	while((sqrLength(_sample) < 4.0) && it <= maxIt)
	{
		_sample = complexSquared(_sample) + _const;
		it++;
	}
	float value = 1 - it / maxIt;

	// Add colours
	int colorIndex = int(value * lastColorIndex);
	vec4 lowColor = colors[colorIndex];
	vec4 highColor = colors[min(lastColorIndex, colorIndex + 1)];

	outColor = mix(lowColor, highColor, value * lastColorIndex - colorIndex);
}