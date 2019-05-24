/*
NG Maths Libratry
Leo Tamminen

Matrix cast operations
*/

#pragma once

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