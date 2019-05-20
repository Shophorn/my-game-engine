#pragma once

#include "../debug.hpp"
#include "MatrixBase.hpp"

#include "vectors.hpp"

namespace ng
{

	template <typename T>
	auto traitStringM ()
	{
		return fmt::format (
			"size: {}, aggregate: {}, pod: {}, standard layout: {}",
			sizeof(T),
			std::is_aggregate_v<T>,
			std::is_pod_v <T>,
			std::is_standard_layout_v<T>
		);
	}

	using float2x2 = maths::MatrixBase<float, 2, 2>;
	using float3x3 = maths::MatrixBase<float, 3, 3>;
	using float4x4 = maths::MatrixBase<float, 4, 4>;

	void testMatrices()
	{
		using namespace maths;
		debug::log("matrix is {}", traitStringM<float4x4>());

		const float2x2 a {1, 0, 0, 1};
		float2x2 b {a[1], 4, 6};

		debug::log("f22: {}", a);
		debug::log("f22: {}", b);
		
		b[1][0] = 9;
		debug::log("set [1][0]: {}", b);

		b.row(0) = a.row(0);
		debug::log("set row (0): {}", b);

		debug::log("get column 0: {}, 1: {}", b.getColumn(0), b.getColumn(1));

		b.setColumn(0, {66, 77});
		b.setColumn(1, {44, 55});
		debug::log("set columns: {}", b);	

		debug::log("transpose of b: {}", b.transpose());

		debug::log("determinant of b: {}", b.determinant());

		debug::log("f22 identity: {}", float2x2::identity());
		debug::log("f33 identity: {}", float3x3::identity());
		debug::log("f44 identity: {}", float4x4::identity());
	}
}