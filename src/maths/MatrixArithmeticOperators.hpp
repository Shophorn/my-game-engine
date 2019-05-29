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
	/*
	Matrix multiplication of compatible matrices.
	*/
	template<typename ValueType, int OuterRows, int InnerSize, int OuterColumns>
	auto operator * (
		MatrixBase<ValueType, OuterRows, InnerSize> 	lhs,
		MatrixBase<ValueType, InnerSize, OuterColumns> 	rhs
	){
		using product_type = MatrixBase<ValueType, OuterRows, OuterColumns>;
		using vector_type = VectorBase<ValueType, InnerSize>;

		product_type product {};
		for (int iCol = 0; iCol < OuterColumns; iCol++)
		{
			auto column = rhs.getColumn(iCol);
			for (int iRow = 0; iRow < OuterRows; iRow++)
			{
				product[iRow][iCol] = dot(lhs.row(iRow), column);
			}
		}

		return product;
	}

	template<typename ValueType, int RowCount, int ColumnCount>
	auto operator * (
		MatrixBase<ValueType, RowCount, ColumnCount> 	mat,
		VectorBase<ValueType, RowCount> 				vec
	){
		using vector_type = decltype(vec);

		if constexpr(RowCount == 2)
			return vector_type {
				dot(mat.row(0), vec),
				dot(mat.row(1), vec)
			};

		if constexpr(RowCount == 3)
			return vector_type {
				dot(mat.row(0), vec),
				dot(mat.row(1), vec),
				dot(mat.row(2), vec)
			};


		if constexpr(RowCount == 4)
			return vector_type {
				dot(mat.row(0), vec),
				dot(mat.row(1), vec),
				dot(mat.row(2), vec),
				dot(mat.row(3), vec)
			};
	}
}