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

		debug::log("Testing matrices:");

		std::array<float, 4> tester;
		debug::log("array size is {}", sizeof(tester));

		MatrixBase<float, 4, 4> matrix;
		debug::log("matrix size is {}", sizeof(matrix));

		debug::log("matrix is {}", traitStringM<float4x4>());

		const float2x2 a {1, 0, 0, 1};
		float2x2 b {a[1], 4, 6};
		
		debug::log("f22: {}", a);
		debug::log("f22: {}", b);
	}
}