/*
Miscellaneous math functions
*/

#pragma once

namespace ng::maths
{
	// Big enough?
	using factorial_type = unsigned long long;

	// Use to get runtime factorial value
	constexpr factorial_type computeFactorial(unsigned n)
	{
		if (n < 0)
			return 0;

		if (n == 1)
			return 1;

		return n * computeFactorial(n - 1);		
	}

	// Compile-time factorial value
	template <unsigned N> constexpr factorial_type factorial = computeFactorial(N);
}