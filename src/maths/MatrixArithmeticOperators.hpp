/*
NG Engine Project
Leo Tamminen

Matrix arithmetic operators
*/

#pragma once

#include "MatrixBase.hpp"
#include "vectors.hpp"

#include "../debug.hpp"

namespace ng::maths
{
	template<typename ValueType, int OuterRows, int InnerSize, int OuterColumns>
	auto operator * (
		MatrixBase<ValueType, OuterRows, InnerSize> lhs,
		MatrixBase<ValueType, InnerSize, OuterColumns> rhs
	){
		using product_type = MatrixBase<ValueType, OuterRows, OuterColumns>;

		using vector_type = VectorBase<ValueType, InnerSize>;

		product_type product {};
		for (int iCol = 0; iCol < OuterColumns; iCol++)
		{
			vector_type column = rhs.getColumn(iCol);
			for (int iRow = 0; iRow < OuterRows; iRow++)
			{
				product[iRow][iCol] = dot(lhs.row(iRow), column);
			}
		}

		return product;
	}
}