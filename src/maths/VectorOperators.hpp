#pragma once

#include "VectorBase.hpp"

template<typename ValueType, int Dimension>
auto operator + (VectorBase<ValueType, Dimension> a, VectorBase<ValueType, Dimension> b)
{
	/*
	Do constexpr checking instead of index accessing.
	If more than 5 element vectors arise, add loop for those elements.
	*/

	if constexpr (Dimension >= 1) a.x += b.x;
	if constexpr (Dimension >= 2) a.y += b.y;
	if constexpr (Dimension >= 3) a.z += b.z;
	if constexpr (Dimension >= 4) a.w += b.w;

	return a;
}

template <typename ValueType, int Dimension>
auto & operator += (VectorBase<ValueType, Dimension> & a, VectorBase<ValueType, Dimension> b)
{
	/*
	Do constexpr checking instead of index accessing.
	If more than 5 element vectors arise, add loop for those elements.
	*/

	if constexpr (Dimension >= 1) a.x += b.x;
	if constexpr (Dimension >= 2) a.y += b.y;
	if constexpr (Dimension >= 3) a.z += b.z;
	if constexpr (Dimension >= 4) a.w += b.w;

	return a;
}