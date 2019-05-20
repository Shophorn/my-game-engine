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
	union VectorBase<ValueType, 2>
	{
		constexpr static int dimension = 2;
		
		using value_type = ValueType;
		using this_type = VectorBase<value_type, dimension>;

		// c++11 promises anonymous structs
		struct
		{
			union { value_type x, r; };
			union { value_type y, g; };
		};
		value_type mData [dimension];

		value_type & operator [] (int index)
		{
			NG_ASSERT (0 <= index && index < dimension);

			return mData [index];
		}

		value_type operator [] (int index) const
		{
			NG_ASSERT (0 <= index && index < dimension);

			return mData [index];
		}
		
		const value_type * valuePtr() const
		{
			return mData;
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