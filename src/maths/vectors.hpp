#pragma once

#include "VectorBase.hpp"
#include "VectorArithmeticOperators.hpp"
#include "VectorOperations.hpp"
#include "VectorCasts.hpp"
#include "Vector2Base.hpp"
#include "Vector3Base.hpp"
#include "Vector4Base.hpp"
#include "VectorFormatting.hpp"

namespace ng
{
	using float2 = maths::Vector2Base<float>;
	using double2 = maths::Vector2Base<double>;

	using float3 = maths::Vector3Base<float>;
	using int3 = maths::Vector3Base<int>;

	using float4 = maths::Vector4Base<float>;

	template <typename T>
	auto traitString ()
	{
		return fmt::format (
			"size: {}, aggregate: {}, pod: {}, standard layout: {}",
			sizeof(T),
			std::is_aggregate_v<T>,
			std::is_pod_v <T>,
			std::is_standard_layout_v<T>
		);
	}

	constexpr float3 up {0, 1, 0};
	constexpr int3 intUp = type_cast<int>(up);

	constexpr float2 up2 = dimension_cast<2>(up);

	void testVectors()
	{	
		using namespace maths;

		debug::log("vector 2 f is {}", traitString<float2>());
		debug::log("vector 3 f is {}", traitString<float3>());
		debug::log("vector 4 f is {}", traitString<float4>());

		// return;
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


		float3 B { 63.2325f, 12.314f, 3.14553f };
		debug::log("cast float3 {} to int3 {}", B, type_cast<int>(B));

		float2 C = dimension_cast<2>(B);
		debug::log("cast float3 to float2: {}, and back {}", C, dimension_cast<3>(C)); 

		// exqisite syntax
		float gg = cross2d<float>({1.0f, 2.0f},{4.2f, 7.1f});
		debug::log("gg:{}", gg);


		float3 h { 6, 7 };
		debug::log("float3 {}", h);

		h[2] = 8;
		debug::log("float3 [2] = 0 --> {}", h);


		// test constexpr things
		debug::log("float3 up: {}, float2 up: {}, int3 up: {}", up, up2, intUp);

		debug::log("shrank float2 from float3: {}", dimension_cast<2>(h));
		debug::log("expanded float4 from float3: {}", dimension_cast<4>(h));

	}
}