#pragma once

#include <type_traits>
#include <array>
#include <utility>

#include "vectors.hpp"
#include "../debug.hpp"
#include "../tmpl.hpp"


namespace ng::maths
{
	template <typename RowVectorType, typename ColumnVectorType>
	struct MatrixBase
	{
		static_assert(
			std::is_same<
				typename ColumnVectorType::value_type,
				typename RowVectorType::value_type
			>::value,
			"'value_type' of both vector types must match");

		static constexpr int columns_count	= RowVectorType::dimension;
		static constexpr int rows_count		= ColumnVectorType::dimension;

		using this_type 		= MatrixBase<RowVectorType, ColumnVectorType>;
		using value_type 		= typename ColumnVectorType::value_type;
		using column_type 		= ColumnVectorType;
		using row_type 			= RowVectorType;
		using transpose_type 	= MatrixBase<ColumnVectorType, RowVectorType>; 
		using row_array_type 	= std::array<row_type, rows_count>;


		// Contents
		row_array_type rows;

		// Vector types' default constructors initialize to zero
		constexpr MatrixBase ()
			: rows (row_array_type {} ) {}

		// Move pre-created rows array
		constexpr MatrixBase (row_array_type&& rows)
			: rows (std::move(rows)) {}

		// Row vector constructor
		// :TODO: I have no idea what typename = std::enable ... syntax means, find out
		// :TODO: need nice errors if should instantiate bad constructor
		template <
			typename ... TArgs, 
			typename = std::enable_if_t<
					tmpl::countIsEqual<rows_count, TArgs...>
				&& 	tmpl::areAllTypeof<row_type, TArgs...>
			>
		>
		constexpr MatrixBase (TArgs&& ... args)
			: rows ( {std::forward<TArgs>(args) ... } ) {}

		/*
		Identity Matrix with 1s on diagonal and zeros elsewhere
		To make this constexpr we have to explicitly define it for each concrete type.
		
		:TODO: this could probably be done with template recursion or similar, but 
		indexing or pointers cannot be used in constexpr context.
		*/
		static constexpr this_type identity() = delete;

		// Return row by indexing
		row_type & operator [] (int index)
		{
			return rows[index];
		}
		
		// Return const rows by indexing
		const row_type & operator [] (int index) const
		{
			return rows[index];
		}

		/*
		Multiply matching vector type from right
		*/
		column_type operator * (row_type vec) const noexcept
		{
			row_type result;
			for (int i = 0; i < rows_count; i++)
			{
				result[i] = row_type::dot(rows[i], vec);
			}
			return result;
		}
		
		/*
		Each call builds new column type vector
		*/
		column_type columns (int index) const noexcept
		{
			column_type result;
			for (int i = 0; i < columns_count; i++)
			{
				result[i] = rows[i][index];
			} 
			return result;
		}

		/*
		Get transpose of this matrix as new object
		*/
		transpose_type transpose() const noexcept
		{
			transpose_type result;
			for (int i = 0; i < columns_count; i++)
			{			
				result[i] = columns(i);
			}
			return result;
		}



		/*
		:TOOD:
			X transpose
			X vector multiplication
			value ptr
			size conversions, type conversion

			perspective functions
		*/
	};

	/*
	Identity definitions.
	:TODO: find nicer way to define these...
	*/
	template<>
	constexpr MatrixBase<float2, float2> MatrixBase<float2, float2>::identity()
	{ 
		return MatrixBase<float2, float2>
		{ 
			float2 {1, 0},
			float2 {0, 1} 
		}; 
	}
	template<>
	constexpr MatrixBase<float3, float3> MatrixBase<float3, float3>::identity()
	{ 
		return MatrixBase<float3, float3>
		{ 
			float3 {1, 0, 0},
			float3 {0, 1, 0},
			float3 {0, 1, 0}
		}; 
	}

	template<>
	constexpr MatrixBase<float4, float4> MatrixBase<float4, float4>::identity()
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

		f33 A {
			float3 (11, 12, 13),
			float3 (21, 22, 23),
			float3 (31, 32, 33)
		};
		debug::log("A {}", A);
		debug::log("transpose of A {}", A.transpose());

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