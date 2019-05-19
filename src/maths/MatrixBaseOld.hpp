#pragma once

#include <array>

#include "../debug.hpp"
#include "../tmpl.hpp"

namespace ng::maths
{
	template <typename RowVectorType, typename ColumnVectorType>
	struct MatrixBase
	{
		/*
		Cannot create matrix of differen value types
		*/
		static_assert(
			std::is_same<
				typename ColumnVectorType::value_type,
				typename RowVectorType::value_type
			>::value,
			"'value_type' of both vector types must match");

		static constexpr int rows_count		= ColumnVectorType::dimension;
		static constexpr int columns_count	= RowVectorType::dimension;

		using this_type 		= MatrixBase<RowVectorType, ColumnVectorType>;
		using value_type 		= typename RowVectorType::value_type;
		using row_type 			= RowVectorType;
		using column_type 		= ColumnVectorType;
		using transpose_type 	= MatrixBase<ColumnVectorType, RowVectorType>; 
		
		// Vector types' default constructors initialize to zero
		constexpr MatrixBase () noexcept = default;

		// Row vector constructor
		// :TODO: I have no idea what typename = std::enable ... syntax means, find out
		// :TODO: need nice errors if should instantiate bad constructor, maybe create false_type constructor
		template <
			typename ... TArgs, 
			typename = std::enable_if_t<
					tmpl::countIsEqual<rows_count, TArgs...>
				&& 	tmpl::areAllTypeof<row_type, TArgs...>
			>
		>
		constexpr MatrixBase (TArgs&& ... args) noexcept
			: mData ( {std::forward<TArgs>(args) ... } ) {}

		/*
		Identity Matrix with 1s on diagonal and zeros elsewhere
		To make this constexpr we have to explicitly define it for each concrete type.
		
		:TODO: this could probably be done with template recursion or similar, but 
		indexing or pointers cannot be used in constexpr context apparently.
		*/
		static constexpr this_type identity() noexcept = delete;

		// Return row by indexing
		row_type & operator [] (int index)
		{
			return mData[index];
		}
		
		// Return const rows by indexing
		const row_type & operator [] (int index) const noexcept
		{
			return mData[index];
		}

		/*
		Get and set rows using mData directly
		*/
		row_type row (int index) const noexcept
		{
			return mData [index];
		}

		void setRow(int rowIndex, row_type value) noexcept
		{
			mData[rowIndex] = value;
		}

		/*
		Get and set columns.
		Each call builds new column type vector, so beware.
		Set loop is probably unrolled by compiler.
		*/
		column_type column (int index) const noexcept
		{
			column_type result;
			for (int i = 0; i < this_type::rows_count; i++)
			{
				result[i] = mData[i][index];
			}
			return result;
		}

		void setColumn(int columnIndex, column_type value) noexcept
		{
			for (int iRow = 0; iRow < rows_count; iRow++)
			{
				mData[iRow][columnIndex] = value[iRow];
			}
		}

		/*
		Get transpose of this matrix as new object
		*/
		transpose_type transpose() const noexcept
		{
			transpose_type result;
			for (int i = 0; i < columns_count; i++)
			{			
				result[i] = column(i);
			}
			return result;
		}

		/*
		Pointer to first element in first row.
		*/
		value_type * valuePtr () noexcept
		{
			return &mData[0][0];	
		}
		/*
		:TOOD:
			inverse

			size conversions, type conversion

			perspective functions
		*/

		/*
		Multiply matching vector type from right
		*/
		column_type operator * (row_type vec) const noexcept
		{
			row_type result;
			for (int i = 0; i < rows_count; i++)
			{
				result[i] = row_type::dot(mData[i], vec);
			}
			return result;
		}
/*
		// lhs::column_type == rhs::row_type
		// return_type = MatrixBase<lhs::row_type, rhs::column_type>
		template<typename OtherColumnType>
		auto operator * (MatrixBase<column_type, OtherColumnType> mat)
		{
			static_assert(std::is_same_v<value_type, typename decltype(mat)::value_type>, "Cannot multiply matrices of different value type");

			using return_type = MatrixBase<row_type, OtherColumnType>;
				
			return_type product {};



			return product;
		}
*/
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



	private:
		/*
		Contents of matrix.
		Do not expose this becouse we may want to change type at some point.
		This expects that vector types default initialize to zero.
		*/
		std::array<row_type, rows_count> mData {};

		/*
		Compute determinant with recursive templates.
		Last level specialization merely returns value at that point, as should be.
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
			int topRow = rows_count - Level;
						
			value_type value = 0;
			int sign = 1;
			for (int i = 0; i < Level; i++)
			{
				value += 
					sign
					* mData[topRow][columns[i]]
					* impl_determinant<nextLevel>(SubArraySkipAtIndex(i, columns));
				sign *= -1;
			}

			return value;
		}

		template<>
		auto impl_determinant<1>(std::array<int, 1> columns) const noexcept
		{
			return mData[rows_count -1][columns[0]];
		}
	};

	/*
	Matrix multiplication operator.
	Defined only when left matrix' column count and right matrix' row count are equal
		(ie. NxM * MxP is defined, NxM * PxS is not)
	
	No need to assert that value types match, since both sides already need to have common
	InnerType for this template to work in the first place.

	NOTE notice stupid template typenames which are counterintuitive. They are due to stupid CRTP -pattern
	used in vector types.
	*/
	template<typename LeftColumnType, typename InnerType, typename RightRowType>
	auto operator * (
		MatrixBase<InnerType, LeftColumnType> lhs,
		MatrixBase<RightRowType, InnerType> rhs
	) {
		using product_type = MatrixBase<LeftColumnType, RightRowType>;

		constexpr int rows = decltype(rhs)::rows_count;
		constexpr int cols = decltype(lhs)::columns_count;

		product_type product {};
		// :TODO: optimise using transpose
		for(int iRow = 0; iRow < rows; iRow++)
		{
			for (int iCol = 0; iCol < cols; iCol++)
			{
				product[iRow][iCol] = InnerType::dot(lhs.row(iRow), rhs.column(iCol));
			}
		}

		return product;
	}
}

#include <sstream>

namespace fmt
{
	template <typename RowType, typename ColumnType>
	struct formatter <ng::maths::MatrixBase<RowType, ColumnType>>
	{
		using type = ng::maths::MatrixBase<RowType, ColumnType>;

		UNUSED_FMT_PARSE

		template <typename FormatContext>
		auto format (const ng::maths::MatrixBase<RowType, ColumnType> & mat, FormatContext &ctx)
		{
			constexpr int rows = type::rows_count;
			constexpr int columns = type::columns_count;

			std::stringstream ss;
			ss << "(";
			for (int iRow = 0; iRow < rows; iRow++)
			{
				for (int iCol = 0; iCol < columns; iCol++)
				{
					ss << mat[iRow][iCol];

					if (iCol < columns - 1)
						ss << ", ";
				}
				if (iRow < rows - 1)
					ss << "; ";

			}
			ss << ")";

			return format_to(ctx.begin(), ss.str());
		}
	};
}