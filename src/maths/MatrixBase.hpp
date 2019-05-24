#pragma once

// #include "vectors.hpp"
#include "VectorBase.hpp"
#include "VectorCasts.hpp"
#include "../tmpl.hpp"

namespace ng::maths
{
	template <typename ValueType, int RowCount, int ColumnCount>
	struct MatrixBase
	{
		static constexpr int count = RowCount * ColumnCount;
		static constexpr int rows_count = RowCount;
		static constexpr int columns_count = ColumnCount;
		static constexpr bool is_square_matrix = (RowCount == ColumnCount);

		using value_type		= ValueType;
		using row_type			= VectorBase<ValueType, ColumnCount>;
		using column_type		= VectorBase<ValueType, RowCount>;
		using this_type			= MatrixBase<ValueType, RowCount, ColumnCount>;
		using transpose_type	= MatrixBase<ValueType, ColumnCount, RowCount>;

		/*
		Return from function because 
		*/
		static this_type identity ()
		{
			static auto identityMatrix = constructIdentity();
			return identityMatrix;
		}

		row_type mRows [rows_count];

		/*
		Access matrix' rows with operator.
		This allows us to use syntax m[0][0] to set and get invidual items.
		*/
		constexpr row_type & operator [] (int index) noexcept
		{
			return mRows[index];
		}

		constexpr row_type operator [] (int index) const noexcept
		{
			return mRows[index];
		}


		/*
		Access rows excplicitly.
		*/
		row_type & row (int index) noexcept
		{
			return mRows [index];
		}

		row_type row (int index) const noexcept
		{
			return mRows [index];
		}

		/*
		Access columns.
		I'm using different style than with rows to make clearer distinction how these
		procedures work.

		Define each dimension separately in get to properly use vector types' aggregate constructors.
		In set loop is fine, since no vector type constructor occurs.
		*/
		column_type getColumn (int index) const noexcept
		{
			if constexpr (rows_count == 2)
				return { mRows[0][index], mRows[1][index] };

			if constexpr (rows_count == 3)
				return { mRows[0][index], mRows[1][index], mRows[2][index] };

			if constexpr (rows_count == 4)
				return { mRows[0][index], mRows[1][index], mRows[2][index], mRows[3][index] };

			// TODO implement more with loops
		}

		void setColumn (int index, column_type value) noexcept
		{
			for (int i = 0; i < rows_count; i++)
			{
				mRows[i][index] = value[i];
			}
		}

		/*
		
		*/
		transpose_type transpose()
		{
			if constexpr (rows_count == 2)
				return { getColumn(0), getColumn(1) };

			if constexpr (rows_count == 3)
				return { getColumn(0), getColumn(1), getColumn(2) };

			if constexpr (rows_count == 4)
				return { getColumn(0), getColumn(1), getColumn(2), getColumn(3) };

			// TODO: implement rest with loop
		}	

		/*
		Compute determinant of matrix.
		Determinant describes relative change in volume of space transformed by matrix.
		Negative determinant means space has been inverted.
		*/
		value_type determinant () const noexcept
		{
			static_assert(
				rows_count == columns_count, 
				"Determinant is only defined for square matrices");

			return impl_determinant<rows_count>();
		}

		/*
		TODO:
			inverse

			vector multiplication
			matrix multiplication
			coeff scalar multiplication (not operator)
			
			submatrix
		*/

		int identityCount = 0;
	private:

		/*
		Construct identityMatrix.
		This should only be used in constexpr context so we dont need to care about proper aggregate
		constructor.
		*/
		static constexpr this_type constructIdentity()
		{
			constexpr int minDimension = rows_count < columns_count ? rows_count : columns_count;
			
			this_type identity{0};
			for (int i = 0; i < minDimension; i++)
			{
				identity[i][i] = 1;
			}
			return identity;
		}

		/*
		Compute determinant with recursive templates.
		First overload with no arguments is used to call with starting column indices (0, 1, ..., Level)
		*/
		template <int Level>
		auto impl_determinant () const noexcept
		{
			return impl_determinant<Level>(tmpl::arraySequence<int, Level>());
		}

		template <int Level>
		auto impl_determinant (std::array<int, Level> columns) const noexcept
		{
			constexpr int nextLevel = Level - 1;
			constexpr int topRow = rows_count - Level;
			
			// TODO			
			// 3x3 matrix using modulus operators, we should use that too instead of SubArraySkipAtIndex() function
			// for(i = 0; i < 3; i++)
		 //    	determinant = determinant + (mat[0][i] * (mat[1][(i+1)%3] * mat[2][(i+2)%3] - mat[1][(i+2)%3] * mat[2][(i+1)%3]));

			value_type value = 0;
			int sign = 1;
			for (int i = 0; i < Level; i++)
			{
				value += 
					sign
					* mRows[topRow][columns[i]]
					* impl_determinant<nextLevel>(SubArraySkipAtIndex(i, columns));
				sign *= -1;
			}

			return value;
		}

		template<>
		auto impl_determinant<2>(std::array<int, 2> columns) const noexcept
		{
			constexpr int topRow = rows_count - 2;

			return
				mRows[topRow][columns[0]] * mRows[topRow + 1][columns[1]]
				- mRows[topRow][columns[1]] * mRows[topRow + 1][columns[0]];
		}



		template<unsigned long long N>
		auto SubArraySkipAtIndex (int skipIndex, std::array<int, N> columns) const
		{
			std::array<int, N - 1>result {};
			for (int iSrc = 0, iDst = 0; iDst < N -1; iSrc++, iDst++)
			{
				if (skipIndex == iSrc)
					iSrc++;

				result [iDst] = columns [iSrc];
			}
			return result; 
		}
	};
}

namespace ng
{
	/*
	Cast matrix to other base type
	*/
	template <typename NewValueType, typename OldValueType, int RowCount, int ColumnCount>
	constexpr auto type_cast (ng::maths::MatrixBase<OldValueType, RowCount, ColumnCount> oldMatrix) noexcept
	{

		if constexpr (std::is_same_v<NewValueType, OldValueType>)
			return oldMatrix;

		using new_type = ng::maths::MatrixBase<NewValueType, RowCount, ColumnCount>;

		// Cast row by row, vector casts with aggregate initialization
		if constexpr(RowCount == 2)
			return new_type {
				type_cast<NewValueType>(oldMatrix.row(0)),
				type_cast<NewValueType>(oldMatrix.row(1))
			};	

		if constexpr(RowCount == 3)
			return new_type {
				type_cast<NewValueType>(oldMatrix.row(0)),
				type_cast<NewValueType>(oldMatrix.row(1)),
				type_cast<NewValueType>(oldMatrix.row(2))
			};	

		if constexpr(RowCount == 4)
			return new_type {
				type_cast<NewValueType>(oldMatrix.row(0)),
				type_cast<NewValueType>(oldMatrix.row(1)),
				type_cast<NewValueType>(oldMatrix.row(2)),
				type_cast<NewValueType>(oldMatrix.row(3))
			};	
	}


	/*
	Cast matrix to different new size
	*/
	template <int NewRowCount, int NewColumnCount, typename ValueType, int OldRowCount, int OldColumnCount>
	constexpr auto size_cast (ng::maths::MatrixBase<ValueType, OldRowCount, OldColumnCount> oldMatrix) noexcept
	{
		if constexpr (NewRowCount == OldRowCount && NewColumnCount == OldColumnCount)
			return oldMatrix;

		using new_type = maths::MatrixBase<ValueType, NewRowCount, NewColumnCount>;

		constexpr int smallestRowCount = tmpl::min<NewRowCount, OldRowCount>;

		if constexpr (smallestRowCount == 2)
			return new_type { 
				dimension_cast<NewColumnCount>(oldMatrix.row(0)), 
				dimension_cast<NewColumnCount>(oldMatrix.row(1))
			};

		if constexpr (smallestRowCount == 3)
			return new_type { 
				dimension_cast<NewColumnCount>(oldMatrix.row(0)), 
				dimension_cast<NewColumnCount>(oldMatrix.row(1)),
				dimension_cast<NewColumnCount>(oldMatrix.row(2))
			};

		if constexpr (smallestRowCount == 4)
			return new_type { 
				dimension_cast<NewColumnCount>(oldMatrix.row(0)), 
				dimension_cast<NewColumnCount>(oldMatrix.row(1)),
				dimension_cast<NewColumnCount>(oldMatrix.row(2)),
				dimension_cast<NewColumnCount>(oldMatrix.row(3))
			};
	}
}
#include <sstream>
namespace fmt
{
	template<typename ValueType, int RowCount, int ColumnCount>
	struct formatter<ng::maths::MatrixBase<ValueType, RowCount, ColumnCount>>
	{
		using matrix_type = ng::maths::MatrixBase<ValueType, RowCount, ColumnCount>;

		UNUSED_FMT_PARSE

		template <typename FormatContext>
		auto format(const matrix_type & matrix, FormatContext & context)
		{
			if constexpr (RowCount == 2)
				return format_to (context.begin(), "({};{})", 
					formatRow(matrix[0]),
					formatRow(matrix[1])
				);

			if constexpr (RowCount == 3)
				return format_to (context.begin(), "({};{};{})", 
					formatRow(matrix[0]),
					formatRow(matrix[1]),
					formatRow(matrix[2])
				);

			if constexpr (RowCount == 4)
				return format_to (context.begin(), "({};{};{};{})", 
					formatRow(matrix[0]),
					formatRow(matrix[1]),
					formatRow(matrix[2]),
					formatRow(matrix[3])
				);

			// TODO: implement more with loop
		}

	private:

		auto formatRow(typename matrix_type::row_type row)
		{
			if constexpr (ColumnCount == 2)
				return fmt::format("{}, {}", row.x, row.y);

			if constexpr (ColumnCount == 3)
				return fmt::format("{}, {}, {}", row.x, row.y, row.z);

			if constexpr (ColumnCount == 4)
				return fmt::format("{}, {}, {}, {}", row.x, row.y, row.z, row.w);
		}
	};
}