/*
NG Maths Library

Template for all matrix sizes.
Relies on VectorBase implementation.

TODO:

	nicify:
		determinant
		adjugate
		inverse
			(1/determinant) * adjugate

	coeff scalar multiplication (not operator)
	
	submatrix
*/

#pragma once

#include "VectorBase.hpp"
#include "VectorCasts.hpp"
#include "MatrixDeterminant.hpp"
#include "../tmpl.hpp"

namespace ng::maths
{
	template <typename ValueType, int RowCount, int ColumnCount>
	struct MatrixBase
	{
		// static constexpr int count = RowCount * ColumnCount;
		static constexpr int rows_count = RowCount;
		static constexpr int columns_count = ColumnCount;
		static constexpr bool is_square_matrix = (RowCount == ColumnCount);

		using value_type		= ValueType;
		using row_type			= VectorBase<ValueType, ColumnCount>;
		using column_type		= VectorBase<ValueType, RowCount>;
		using this_type			= MatrixBase<ValueType, RowCount, ColumnCount>;
		using transpose_type	= MatrixBase<ValueType, ColumnCount, RowCount>;

		/*
		Identity matrix, 1s on diagonal, 0s elsewhere.
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
		Transpose of matrix.
		Currently only works correctly for square matrices of sizes 2 - 4
		*/
		transpose_type transpose()
		{
			static_assert(is_square_matrix, "Only square matrices 2 - 4 implemented now");

			if constexpr (rows_count == 2)
				return { getColumn(0), getColumn(1) };

			if constexpr (rows_count == 3)
				return { getColumn(0), getColumn(1), getColumn(2) };

			if constexpr (rows_count == 4)
				return { getColumn(0), getColumn(1), getColumn(2), getColumn(3) };

			// TODO: implement more dimensions, as well as different shapes with loop
		}	

		/*
		Compute determinant of matrix.
		Determinant describes relative change in volume of space transformed by matrix.
		Negative determinant means space has been inverted.
		*/
		value_type determinant () const noexcept
		{
			static_assert(
				is_square_matrix, 
				"Determinant is only defined for square matrices");

			// This is not good function :(
			// too confusing and seems like compile time recursive but it is not.
			return impl_determinant<rows_count>();
		}

		this_type adjugate () const noexcept
		{
			static_assert (
				is_square_matrix, 
				"Adjugate if defined for square matrices only");

			return impl_adjugate();
		}

		this_type inverse () const noexcept
		{
			static_assert (
				is_square_matrix,
				"Must be square matrix");

			auto inv = adjugate();
			inv.coeffMultiply(static_cast<value_type>(1.0) / determinant());
			return inv;
		}

		void coeffMultiply(value_type value)
		{
			for (int r = 0; r < rows_count; r++)
			{
				for (int c = 0; c < columns_count; c++)
				{
					mRows[r][c] *= value;
				}
			}
			// return *this;
		}

		/*
		Trace is sum of elements on main diagonal.
		*/
		value_type trace () const noexcept
		{
			value_type value = 0;
			for (int i = 0; i < rows_count; i++)
			{
				value += mRows[i][i];
			}
			return value;
		}

	private:

		/*
		Construct identityMatrix.
		This should only be used in constexpr context so we dont need to care about proper aggregate
		constructor.
		*/
		static this_type constructIdentity()
		{
			constexpr int minDimension = tmpl::min<rows_count, columns_count>;
			
			this_type identity {};
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
		auto impl_determinant (std::array<int, Level> cofactorColumns) const noexcept
		{
			constexpr int nextLevel = Level - 1;
			constexpr int topRow = rows_count - Level;
			
			value_type value = 0;
			// BUG: wrong???
			int sign = 1; // (-1)^(row + column) ???
			for (int i = 0; i < Level; i++)
			{
				value += 
					sign
					* mRows[topRow][cofactorColumns[i]]
					* impl_determinant<nextLevel>(GetCofactorColumns(i, cofactorColumns));
				sign *= -1;
			}

			return value;
		}

		template<>
		auto impl_determinant<2>(std::array<int, 2> cofactorColumns) const noexcept
		{
			constexpr int topRow = rows_count - 2;

			return
				mRows[topRow][cofactorColumns[0]] * mRows[topRow + 1][cofactorColumns[1]]
				- mRows[topRow][cofactorColumns[1]] * mRows[topRow + 1][cofactorColumns[0]];
		}

		template<unsigned long long N>
		auto GetCofactorColumns (int skipIndex, std::array<int, N> columns) const
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


		///////////////////////////
		/// Adjugate 			///
		///////////////////////////

		auto getCofactor(int row, int column) const noexcept
		{
			constexpr int cofactor_rows_count = rows_count - 1;
			constexpr int cofactor_columns_count = columns_count -1;

			using cofactor_type = MatrixBase<
									value_type, 
									cofactor_rows_count,
									cofactor_columns_count
								>;

			cofactor_type c;


			for (int iRow = 0, iRowCofactor = 0; iRowCofactor < cofactor_rows_count; iRow++, iRowCofactor++)
			{
				if (iRow == row)
					iRow++;

				for (int iCol = 0, iColCofactor = 0; iColCofactor < cofactor_columns_count; iCol++, iColCofactor++)
				{
					
					if (iCol == column)
						iCol++;

					c[iRowCofactor][iColCofactor] = mRows[iRow][iCol];
				}
			}

			return c;
		}

		auto impl_adjugate() const noexcept
		{
			if constexpr (rows_count == 2)
			{
				return this_type
				{
					mRows[1][1], -1 * mRows[0][1],
					-1 * mRows[1][0], mRows[0][0] 
				};
			}
			else
			{
				this_type cofactorMatrix;

				for (int iRow = 0; iRow < rows_count; iRow++)
				{
					for (int iCol = 0; iCol < columns_count; iCol++)
					{
						int sign = pow(-1, iRow + iCol);

						cofactorMatrix [iRow][iCol] = sign * getCofactor(iRow, iCol).determinant();
					}
				}

				return cofactorMatrix.transpose();
			}
		}
	};
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