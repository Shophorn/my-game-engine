#pragma once

#include "VectorBase.hpp"
#include "VectorArithmeticOperators.hpp"
#include "VectorOperations.hpp"
#include "Vector2Base.hpp"
#include "Vector3Base.hpp"
#include "Vector4Base.hpp"
#include "VectorFormatting.hpp"

using float2 = Vector2Base<float>;
using double2 = Vector2Base<double>;

using float3 = Vector3Base<float>;
using int3 = Vector3Base<int>;

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

	// aa should unmodified

	debug::log("operator += : {}, +{}", aa += bb, bb);
	debug::log("operator -= : {}, -{}", aa -= bb, bb);
	debug::log("operator *= : {}, *{}", aa *= f, f);
	debug::log("operator /= : {}, /{}", aa /= f, f);

	float2 cc {1, 0};
	float2 dd {0, 1};
	float2 ee {0.5f, 0.5f};

	debug::log("dot ({}, {}) = {}", cc, dd, dot(cc, dd));
	debug::log("dot ({}, {}) = {}", cc, ee, dot(cc, ee));
	debug::log("dot ({}, {}) = {}", dd, ee, dot(dd, ee));

	debug::log("cross2d ({}, {}) = {}", cc, dd, cross2d(cc, dd));
	debug::log("cross2d ({}, {}) = {}", cc, ee, cross2d(cc, ee));
	debug::log("cross2d ({}, {}) = {}", dd, ee, cross2d(dd, ee));

	debug::log("magnitude ({}) = {}", cc, magnitude(cc));
	debug::log("magnitude ({}) = {}", dd, magnitude(dd));
	debug::log("magnitude ({}) = {}", ee, magnitude(ee));

	debug::log("sqrMagnitude ({}) = {}", cc, sqrMagnitude(cc));
	debug::log("sqrMagnitude ({}) = {}", dd, sqrMagnitude(dd));
	debug::log("sqrMagnitude ({}) = {}", ee, sqrMagnitude(ee));

	debug::log("normalize ({}) = {}", cc, normalize(cc));
	debug::log("normalize ({}) = {}", dd, normalize(dd));
	debug::log("normalize ({}) = {}", ee, normalize(ee));

	int3 A {9, 1, 4};
	debug::log("int3 {}", A);
	debug::log("magnitude {}, sqrMagnitude {}, float magnitude {}", magnitude(A), sqrMagnitude(A), magnitude(float3{9, 1, 4}));


	// exqisite syntax
	float gg = cross2d<float>({1.0f, 2.0f},{4.2f, 7.1f});
	debug::log("gg:{}", gg);
}