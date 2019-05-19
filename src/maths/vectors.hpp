#pragma once

#include "VectorBase.hpp"
#include "VectorOperators.hpp"
#include "Vector2Base.hpp"
#include "Vector3Base.hpp"
#include "Vector4Base.hpp"


using float2 = Vector2Base<float>;
using double2 = Vector2Base<double>;

using float3 = Vector3Base<float>;

void testVectors2()
{
	using namespace ng;

	float2 a (0, 7);
	float2 b (1);
	b += a;
	float2 c = a + b;
	float2 d (1.0f, 5);

	debug::log("{}\n{}\n{}\n{}", a, b, c, d);
}