#pragma once

#include "VectorBase.hpp"

// :TODO: check auto type deductions

/*
Do constexpr checking instead of index accessing, since indexing is now rather slow.
If more than 5 element vectors arise, add loop for those elements.

Pass arguments by value, they can be moved to also. Vector types are quite small, so its
okay to copy them.
*/

namespace ng::maths
{
	// Vector - Vector operators --------------------------------------------------

	// Addition +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
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

	// Subtraction ----------------------------------------------------------------
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
		VectorBase<ValueType, Dimension> a,
		VectorBase<ValueType, Dimension> b
	) {
		return a -= b;
	}


	// Vector - Value operators ---------------------------------------------------

	// Multiplication *************************************************************
	template <typename ValueType, int Dimension>
	auto & operator *= (
		VectorBase<ValueType, Dimension> & vec,
		ValueType value
	){
		if constexpr (Dimension >= 2)
		{
			vec.x *= value;
			vec.y *= value;
		}

		if constexpr (Dimension >= 3) vec.z *= value;
		if constexpr (Dimension >= 4) vec.w *= value;

		return vec;
	}

	template <typename ValueType, int Dimension>
	auto operator * (
		VectorBase<ValueType, Dimension> vec,
		ValueType value
	) {
		return vec *= value;	
	}

	// Division ///////////////////////////////////////////////////////////////////
	template <typename ValueType, int Dimension>
	auto & operator /= (
		VectorBase<ValueType, Dimension> & vec,
		ValueType value
	){
		if constexpr (Dimension >= 2)
		{
			vec.x /= value;
			vec.y /= value;
		}

		if constexpr (Dimension >= 3) vec.z /= value;
		if constexpr (Dimension >= 4) vec.w /= value;

		return vec;
	}

	template <typename ValueType, int Dimension>
	auto operator / (
		VectorBase<ValueType, Dimension> vec,
		ValueType value
	){
		return vec /= value;
	}
}