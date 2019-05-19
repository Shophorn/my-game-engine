#pragma once

namespace ng
{
	template<typename NewValueType, typename OldValueType, int Dimension>
	auto type_cast(maths::VectorBase<OldValueType, Dimension> oldVec)
	{
		if constexpr (std::is_same_v<NewValueType, OldValueType>)
			return oldVec;

		using new_type = maths::VectorBase<NewValueType, Dimension>;

		if constexpr (Dimension == 2)
			return new_type {
				static_cast<NewValueType>(oldVec.x),
				static_cast<NewValueType>(oldVec.y)
			};

		if constexpr (Dimension == 3)
			return new_type {
				static_cast<NewValueType>(oldVec.x),
				static_cast<NewValueType>(oldVec.y),
				static_cast<NewValueType>(oldVec.z)
			};

		if constexpr (Dimension == 4)
			return new_type {
				static_cast<NewValueType>(oldVec.x),
				static_cast<NewValueType>(oldVec.y),
				static_cast<NewValueType>(oldVec.z),
				static_cast<NewValueType>(oldVec.w)
			};
	}

	template <int NewDimension, typename ValueType, int OldDimension>
	auto dimension_cast(maths::VectorBase<ValueType, OldDimension> oldVec)
	{
		if constexpr (NewDimension == OldDimension)
			return oldVec;

		maths::VectorBase<ValueType, NewDimension> newVec {0};

		if constexpr (NewDimension >= 1 && OldDimension >= 1) newVec.x = oldVec.x;
		if constexpr (NewDimension >= 2 && OldDimension >= 2) newVec.y = oldVec.y;
		if constexpr (NewDimension >= 3 && OldDimension >= 3) newVec.z = oldVec.z;
		if constexpr (NewDimension >= 4 && OldDimension >= 4) newVec.w = oldVec.w;

		return newVec;
	}
}	