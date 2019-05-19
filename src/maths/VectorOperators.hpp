#pragma once

#include "VectorBase.hpp"

// :TODO: check auto type deductions

/*
Do constexpr checking instead of index accessing, since indexing is now rather slow.
If more than 5 element vectors arise, add loop for those elements.

Pass arguments by value, they can be moved to also.
*/


template <typename ValueType, int Dimension>
auto & operator += (
	VectorBase<ValueType, Dimension> & a,
	VectorBase<ValueType, Dimension> b
){
	if constexpr (Dimension >= 2)
	{
		a.x += b.x;
		a.y += b.y;
	}

	if constexpr (Dimension >= 3) a.z += b.z;
	if constexpr (Dimension >= 4) a.w += b.w;

	return a;
}

template<typename ValueType, int Dimension>
auto operator + (
	VectorBase<ValueType, Dimension> a,
	VectorBase<ValueType, Dimension> b
){
	return a += b;
}

template <typename ValueType, int Dimension>
auto & operator -= (
	VectorBase<ValueType, Dimension> & a,
	VectorBase<ValueType, Dimension> b
){
	if constexpr (Dimension >= 2)
	{
		a.y -= b.y;
		a.x -= b.x;
	}

	if constexpr (Dimension >= 3) a.z -= b.z;
	if constexpr (Dimension >= 4) a.w -= b.w;

	return a;
}

template <typename ValueType, int Dimension>
auto operator - (
	VectorBase<ValueType, Dimension> & a,
	VectorBase<ValueType, Dimension> b
) {
	return a -= b;
}