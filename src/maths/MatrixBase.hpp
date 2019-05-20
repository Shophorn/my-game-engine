#pragma once

// #include "vectors.hpp"
#include "VectorBase.hpp"

namespace ng::maths
{
	template <typename ValueType, int RowCount, int ColumnCount>
	struct MatrixBase
	{
		static constexpr int count = RowCount * ColumnCount;
		static constexpr int rows_count = RowCount;
		static constexpr int columns_count = ColumnCount;

		using value_type	= ValueType;
		using row_type		= VectorBase<ValueType, ColumnCount>;
		using column_type	= VectorBase<ValueType, RowCount>;
		using this_type		= MatrixBase<ValueType, RowCount, ColumnCount>;

		row_type mRows [rows_count];

		constexpr row_type & operator [] (int index)
		{
			return mRows[index];
		}

		constexpr row_type operator [] (int index) const
		{
			return mRows[index];
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

		auto formatRow(typename matrix_type::row_type row)
		{
			if constexpr (ColumnCount == 2)
				return fmt::format("{}, {}", row.x, row.y);

			if constexpr (ColumnCount == 3)
				return fmt::format("{}, {}, {}", row.x, row.y, row.z);

			if constexpr (ColumnCount == 4)
				return fmt::format("{}, {}, {}, {}", row.x, row.y, row.z, row.w);
		}

		template <typename FormatContext>
		auto format(const matrix_type & matrix, FormatContext & context)
		{
			// if constexpr (RowCount > 4 || ColumnCount > 4)
			// 	return format_to (context.begin(), "matrix({}:{} x {})",
			// 		typeid(ValueType).name(),
			// 		RowCount,
			// 		ColumnCount
			// 	);

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



			// std::stringstream ss;
			// ss << "(";
			// for (int iRow = 0; iRow < RowCount; iRow++)
			// {
			// 	for (int iCol = 0; iCol < ColumnCount; iCol++)
			// 	{
			// 		// ss << 
			// 	}
			// }
		}
	};
}