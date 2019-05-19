#pragma once

#include <cmath>

#include "VectorBase.hpp"

template<typename ValueType, int Dimension>
ValueType dot (
	VectorBase<ValueType, Dimension> a,
	VectorBase<ValueType, Dimension> b
){
	// 2d
	if constexpr (Dimension == 2)
		return sqrt(a.x * b.x + a.y * b.y);

	// 3d
	if constexpr (Dimension == 3)
		return sqrt(a.x * b.x + a.y * b.y + a.z * b.z);
	
	// 4d
	if constexpr (Dimension == 4)
		return sqrt(a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w);
}

template <typename ValueType, int Dimension>
ValueType magnitude (VectorBase<ValueType, Dimension> vec)
{
	// 2d
	if constexpr (Dimension == 2)
		return sqrt(vec.x * vec.x + vec.y * vec.y);

	// 3d
	if constexpr (Dimension == 3)
		return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	
	// 4d
	if constexpr (Dimension == 4)
		return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z + vec.w * vec.w);
}

template <typename ValueType, int Dimension>
ValueType sqrMagnitude (VectorBase<ValueType, Dimension> vec)
{
	// 2d
	if constexpr (Dimension == 2)
		return vec.x * vec.x + vec.y * vec.y;
	
	// 3d
	if constexpr (Dimension == 3)
		return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;

	// 4d
	if constexpr (Dimension == 4)
		return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z + vec.w * vec.w;
}

/*
Only defined for floating point vectors
*/
template <typename ValueType, int Dimension>
auto normalize(VectorBase<ValueType, Dimension> vec)
{
	static_assert(std::is_floating_point_v<ValueType>,
		"Normalizing non-floating point vector wouldn't make sense, so it is not supported");

	return vec / magnitude(vec);
}