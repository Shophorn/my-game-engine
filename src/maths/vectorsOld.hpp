// /*
// NG engine project
// Leo Tamminen

// Concrete vector types
// Debug formatters for vector types
// */

// #pragma once

// #include "VectorBase.hpp"

// // conrete implementations
// struct float2 : ng::maths::VectorBase<float, 2, float2>
// {
// 	union { float x = 0, r; };
// 	float y = 0;

// 	constexpr float2 () = default;
// 	constexpr float2 (float _x, float _y) : x(_x), y(_y) {}
// };

// struct float3 : ng::maths::VectorBase<float, 3, float3>
// {
// 	float x = 0;
// 	float y = 0;
// 	float z = 0;

// 	constexpr float3 () = default;
// 	constexpr float3 (float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
// };

// struct float4 : ng::maths::VectorBase<float, 4, float4>
// {
// 	float x = 0;
// 	float y = 0;
// 	float z = 0;
// 	float w = 0;

// 	constexpr float4 () = default;
// 	constexpr float4 (float x, float y, float z, float w) 
// 		: x(x), y(y), z(z), w(w) {}
// };

// struct double2 : ng::maths::VectorBase<double, 2, double2>
// {
// 	double x = 0;
// 	double y = 0;

// 	constexpr double2 () = default;
// 	constexpr double2 (float _x, float _y) : x(_x), y(_y) {}
// };

// struct int2 : ng::maths::VectorBase<int, 2, int2>
// {
// 	int x = 0;
// 	int y = 0;

// 	constexpr int2 () = default;
// 	constexpr int2 (int _x, int _y) : x(_x), y(_y) {}
// };


// #include <fmt/format.h>

// /*
// Stupid multiline macro. Escape slashes **MUST** be last characters here to
// split macro on multiple lines. No invisible whitespace allowed!!
// */ 
// #define UNUSED_FMT_PARSE 							\
// 		template <typename ParseContext> 			\
// 		constexpr auto parse (ParseContext &ctx) 	\
// 		{ return ctx.begin(); }

// #include <sstream>

// namespace fmt
// {
// 	// template <typename V, int N, typename D> 
// 	// struct formatter <ng::maths::VectorBase<V, N, D>>
// 	// {
// 	// 	UNUSED_FMT_PARSE

// 	// 	template <typename FormatContext>
// 	// 	auto format(const D & vec, FormatContext & ctx)
// 	// 	{
// 	// 		std::stringstream ss;
// 	// 		ss << "(";
// 	// 		for (int i = 0; i < N; i ++)
// 	// 		{
// 	// 			ss << vec[i];

// 	// 			if (i < N -1)
// 	// 				ss << ", ";
// 	// 		}
// 	// 		ss << ")";

// 	// 		return format_to(ctx.begin(), ss.str());
// 	// 	}
// 	// };

// 	template <> struct formatter <float2>
// 	{
// 		UNUSED_FMT_PARSE

// 		template <typename FormatContext>
// 		auto format (const float2 & f2, FormatContext &ctx)
// 		{
// 			return format_to(ctx.begin(), "float2 ({:.1f}, {:.1f})", f2.x, f2.y);
// 		}
// 	};

// 	template <> struct formatter <float3>
// 	{
// 		UNUSED_FMT_PARSE

// 		template <typename FormatContext>
// 		auto format (const float3 & f3, FormatContext &ctx)
// 		{
// 			return format_to(
// 				ctx.begin(),
// 				"float3 ({:.1f}, {:.1f}, {:.1f})",
// 				f3.x,
// 				f3.y,
// 				f3.z
// 			);
// 		}
// 	};

// 	template <> struct formatter <int2>
// 	{
// 		UNUSED_FMT_PARSE
		
// 		template <typename FormatContext>
// 		auto format (const int2 & i2, FormatContext &ctx)
// 		{
// 			return format_to(ctx.begin(), "int2 ({}, {})", i2.x, i2.y);
// 		}
// 	};
// }