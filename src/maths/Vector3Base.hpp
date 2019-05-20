#pragma once

#include "VectorBase.hpp"
#include "../assertions.hpp"

namespace ng::maths
{
	/*
	Base alias for 3d vector types.
	*/
	template<typename T>
	using Vector3Base = VectorBase<T, 3>;

	template<typename ValueType>
	union VectorBase<ValueType, 3>
	{
		constexpr static int dimension = 3;
		
		using value_type = ValueType;
		using this_type = VectorBase<value_type, dimension>;

		struct
		{
			union { value_type x, r; };
			union { value_type y, g; };
			union { value_type z, b; };
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

	template <typename ValueType>
	auto cross(
		Vector3Base<ValueType> lhs,
		Vector3Base<ValueType> rhs
	){
		return Vector3Base<ValueType> 
		{
			lhs.y * rhs.z - lhs.z * rhs.y,
			lhs.z * rhs.x - lhs.x * rhs.z,
	 		lhs.x * rhs.y - lhs.y * rhs.x
		};
	}
}