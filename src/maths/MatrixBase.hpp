#pragma once

#include <type_traits>
#include <array>
// #include <utility>

#include "vectors.hpp"
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
			determinant
			inverse

			size conversions, type conversion

			perspective functions
		*/


		value_type determinant () const noexcept
		{
			static_assert(
				rows_count == columns_count, 
				"Determinant is only defined for square matrices");

			if (rows_count == 3)
				return det<3>(0, 1, 2);
			return det <row_type::dimension>();
		}

		template <int Level, typename ... Columns>
		auto det(Columns ... columns) const
		{
			int topRow = rows_count - Level;

			columnGenerator<Level> gen (columns...);

			value_type value = 0;
			int sign = 1;
			for (int i = 0; i < Level; i++)
			{
				value +=
					sign 
					* mData[topRow][gen.columns[i]] 
					* det<Level - 1>(gen.next(), gen.next());
				sign *= -1;
			}
			return value;	
		}

		template <> auto det<2> () const { return det<2>(0, 1); }
 		template <> auto det<2, int, int> (int a, int b) const
		{
			int topRow = rows_count - 2;

			return
				mData[topRow][a] * mData[rows_count - 1][b]
				- mData[topRow][b] * mData[rows_count - 1][a];  
		}	
/*
		template <> auto det<3> () const { return det<3> (0, 1, 2); }
		template <> auto det<3> (int a, int b, int c) const
		{
			constexpr int level = 3;
			int topRow = rows_count - level;

			columnGenerator<level> gen (a, b, c);

			value_type value = 0;
			int sign = 1;
			for (int i = 0; i < level; i++)
			{
				value +=
					sign 
					* mData[topRow][gen.columns[i]] 
					* det<level - 1>(gen.next(), gen.next());
				sign *= -1;
			}
			return value;
		}
*/
		template<> auto det<4> () const { return det<4>(0, 1, 2, 3); }
		template<> auto det<4> (int a, int b, int c, int d) const
		{
			constexpr int level = 4;
			int topRow = rows_count - level;

			columnGenerator<level> gen (a, b, c, d);

			value_type value = 0;
			int sign = 1;
			for (int i = 0; i < level; i++)
			{
				value += 
					sign 
					* mData[topRow][gen.columns[i]]
					* det<level - 1>(gen.next(), gen.next(), gen.next());
				sign *= -1;
			}
			return value;
		}

		template <int Range>
		struct columnGenerator
		{
			std::array<int, Range> columns;

			template <typename ... Cols>
			columnGenerator (Cols ... cols)
				: columns (decltype(columns) { cols... } )
				{
					// for (int i = 0; i < Range; i++)
						// debug::log("{}", columns[i]);
				}

			int next()
			{
				state++;
				if ((state % Range) == (state / Range))
					state++;

				int index = state % Range;
				int col = columns[index];
				// debug::log("{}, {}", index, col);
				return columns[state % Range];
			}

		private:
			int state = 0;
		};

		struct skipDiagonalGenerator
		{
			int state = 0;
			bool done = false;

			int	range;
			int count;

			skipDiagonalGenerator (int range)
				: range (range), count (range * range) {}

			operator bool()
			{
				return !done;
			}

			int next()
			{
				state++;
				
				int col = state % range;
				int row = state / range;

				if (row == range && col == range)
				{
					done = true;
				}

				if (row == col)
				{
					state++;
					col = state % range;
					row = state / range;

				}

				return col;
			}
		};


	private:
		// Contents
		// Do not expose this becouse we may want to change type at some point
		std::array<row_type, rows_count> mData {};
	};

	/*
	Identity definitions.
	:TODO: find nicer way to define these...
	*/



}

#include <sstream>

namespace fmt
{
	template <typename C, typename R>
	struct formatter <ng::maths::MatrixBase<C, R>>
	{
		UNUSED_FMT_PARSE

		template <typename FormatContext>
		auto format (const ng::maths::MatrixBase<C, R> & mat, FormatContext &ctx)
		{
			constexpr int columns = C::dimension;
			constexpr int rows = R::dimension;

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