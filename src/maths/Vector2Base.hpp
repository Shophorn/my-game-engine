#pragma once

#include "VectorBase.hpp"
#include "../assertions.hpp"

namespace ng::maths
{
	/*
	Base alias for 2d vector types
	*/
	template<typename T>
	using Vector2Base = VectorBase<T, 2>;

	template<typename ValueType>
	struct VectorBase<ValueType, 2>
	{
		using value_type = ValueType;
		constexpr static int dimension = 2;

		// data members
		union { value_type x, r; };
		union { value_type y, g; };

		constexpr value_type & operator [] (int index)
		{
			NG_ASSERT (0 <= index && index < dimension);

			return (&x)[index];
		}

		constexpr value_type operator [] (int index) const
		{
			NG_ASSERT (0 <= index && index < dimension);

			return (&x)[index];
		}
		
		const value_type * valuePtr() const
		{
			return &x;
		}	
	};

	/*
	Two-dimensional cross product.
	*/
	template<typename ValueType>
	auto cross2d (
		Vector2Base<ValueType> lhs,
		Vector2Base<ValueType> rhs
	){
		return lhs.x * rhs.y - lhs.y * rhs.x;
	}
}