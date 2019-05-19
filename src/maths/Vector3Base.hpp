#pragma once

#include "VectorBase.hpp"
#include "../assertions.hpp"

/*
Base alias for 3d vector types.
*/
template<typename T>
using Vector3Base = VectorBase<T, 3>;

template<typename ValueType>
struct VectorBase<ValueType, 3>
{
	constexpr static int dimension = 3;
	
	using value_type = ValueType;
	using this_type = VectorBase<value_type, dimension>;

	union { value_type x, r; };
	union { value_type y, g; };
	union { value_type z, b; };

	// No initialize constructor
	constexpr VectorBase() = default;

	explicit constexpr VectorBase (value_type v)
		: x(v), y(v), z(v) {}

	constexpr VectorBase(value_type x, value_type y, value_type z)
		: x(x), y(y), z(z) {}


	value_type & operator [] (int index)
	{
		NG_ASSERT (0 <= index && index < dimension);

		/*
		Writing first entry as default supresses no return warning,
		since we already asserted that index is valid.
		*/
		switch (index)
		{
			default: return x;
			case 1: return y;
			case 2: return z;
		}
	}

	value_type operator [] (int index) const
	{
		NG_ASSERT (0 <= index && index < dimension);

		switch (index)
		{
			default: return x;
			case 1: return y;
			case 2: return z;
		}
	}

	const value_type * valuePtr() const
	{
		return reinterpret_cast<const value_type*>(this);
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