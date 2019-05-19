#pragma once

#include "VectorBase.hpp"
#include "../assertions.hpp"


/*
Base alias for 2d vector types
*/
template<typename T>
using Vector2Base = VectorBase<T, 2>;

template<typename ValueType>
struct VectorBase<ValueType, 2>
{
	constexpr static int dimension = 2;
	
	using value_type = ValueType;
	using this_type = VectorBase<value_type, dimension>;

	union { value_type x, r; };
	union { value_type y, g; };

	// No initialize constructor
	// TODO: Why do we need to define this, but not other default constructors
	constexpr VectorBase() = default;

	explicit constexpr VectorBase (value_type v)
		: x(v), y(v) {}

	constexpr VectorBase(value_type x, value_type y)
		: x(x), y(y) {}


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
		}
	}

	value_type operator [] (int index) const
	{
		NG_ASSERT (0 <= index && index < dimension);

		switch (index)
		{
			default: return x;
			case 1: return y;
		}
	}
	
	/*
	Only return const pointers, since i have no idea of memory layout, so we shouldn't write
	Seems to work fine though
	*/
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