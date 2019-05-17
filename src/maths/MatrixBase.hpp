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

	private:
		// Contents
		// Do not expose this becouse we may want to change type at some point
		std::array<row_type, rows_count> mData {};
	};

	/*
	Identity definitions.
	:TODO: find nicer way to define these...
	*/
	template<>
	constexpr MatrixBase<float2, float2> MatrixBase<float2, float2>::identity() noexcept
	{ 
		return MatrixBase<float2, float2>
		{ 
			float2 {1, 0},
			float2 {0, 1} 
		}; 
	}
	template<>
	constexpr MatrixBase<float3, float3> MatrixBase<float3, float3>::identity() noexcept
	{ 
		return MatrixBase<float3, float3>
		{ 
			float3 {1, 0, 0},
			float3 {0, 1, 0},
			float3 {0, 1, 0}
		}; 
	}

	template<>
	constexpr MatrixBase<float4, float4> MatrixBase<float4, float4>::identity() noexcept
	{
		return MatrixBase<float4, float4>
		{
			float4 {1, 0, 0, 0},
			float4 {0, 1, 0, 0},
			float4 {0, 0, 1, 0},
			float4 {0, 0, 0, 1}
		};
	}

	/*
	Concrete type definitions
	*/
	using f22 = MatrixBase<float2, float2>;
	using f33 = MatrixBase<float3, float3>;
	using f44 = MatrixBase<float4, float4>;

	void testMatrices()
	{
		debug::log("f22::identity = {}", f22::identity());
		debug::log("f33::identity = {}", f33::identity());
		debug::log("f44::identity = {}", f44::identity());

		f33 M {
			float3 (11, 12, 13),
			float3 (21, 22, 23),
			float3 (31, 32, 33)
		};
		debug::log("M {}", M);
		debug::log("transpose of M {}", M.transpose());
		debug::log("transpose of transpose of M {}", M.transpose().transpose());

		M.setColumn(1, float3(0,0,0));
		M.setRow(1, float3(1, 1, 1));

		debug::log("M {}", M);

		return;
	}
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