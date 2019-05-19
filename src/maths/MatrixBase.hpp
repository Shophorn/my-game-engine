#pragma once

#include "VectorBase.hpp"
#include <array>

namespace ng::maths
{
	template <typename ValueType, int RowCount, int ColumnCount>
	union MatrixBase
	{
		static constexpr int count = RowCount * ColumnCount;
		static constexpr int rows_count = RowCount;
		static constexpr int columns_count = ColumnCount;

		using value_type	= ValueType;
		using row_type		= VectorBase<ValueType, ColumnCount>;
		using column_type	= VectorBase<ValueType, RowCount>;
		using this_type		= MatrixBase<ValueType, RowCount, ColumnCount>;

		// data members
		std::array <value_type, count> mValues;
		std::array <row_type, rows_count> mRows;

		value_type & operator [] (int row)
		{

		}
	};
}