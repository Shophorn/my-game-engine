#pragma once

#include "../debug.hpp"
#include "MatrixBase.hpp"

#include <array>

namespace ng
{




	void testMatrices()
	{
		using namespace maths;

		debug::log("Testing matrices:");

		std::array<float, 4> tester;
		debug::log("array size is {}", sizeof(tester));

		MatrixBase<float, 4, 4> matrix;
		debug::log("matrix size is {}", sizeof(matrix));

		debug::log("matrix is aggregate: {}, pod: {}",
			std::is_aggregate_v<MatrixBase<float,4,4>>,
			std::is_pod_v<MatrixBase<float,4,4>>);
	}
}