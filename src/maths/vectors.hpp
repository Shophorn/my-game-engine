#pragma once

#include "VectorBase.hpp"
#include "VectorArithmeticOperators.hpp"
#include "Vector2Base.hpp"
#include "Vector3Base.hpp"
#include "Vector4Base.hpp"


using float2 = Vector2Base<float>;
using double2 = Vector2Base<double>;

using float3 = Vector3Base<float>;

void testVectors2()
{
	using namespace ng;

	float2 a{};
	float2 b {22};
	float2 c {11, 22};
	float2 d {c};
	float2 e {std::move(c)};

	debug::log("default constructor: {}", a);
	debug::log("single value constructor: {}", b);
	debug::log("all values constructor: {}", c);
	debug::log("copy constructor: {}", d);
	debug::log("move constructor: {}", e);

	float2 aa {1};
	float2 bb {2, 5};
	float f = 3;

	debug::log("operator + : {}, {}", aa + bb, aa);
	debug::log("operator - : {}, {}", aa - bb, aa);
	debug::log("operator * : {}, {}", aa * f, aa);
	debug::log("operator / : {}, {}", aa / f, aa);

	// aa = float2 (1);

	debug::log("operator += : {}, +{}", aa += bb, bb);
	debug::log("operator -= : {}, -{}", aa -= bb, bb);
	debug::log("operator *= : {}, *{}", aa *= f, f);
	debug::log("operator /= : {}, /{}", aa /= f, f);
}