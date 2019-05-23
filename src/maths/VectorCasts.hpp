#pragma once

namespace ng
{
	/*
	Implement 2 - 4 dimensions explictly so that we can take use of aggregate initialization.
	*/


	/*
	Cast vector to a vector of same size but different type.
	*/
	template<typename NewValueType, typename OldValueType, int Dimension>
	constexpr auto type_cast(maths::VectorBase<OldValueType, Dimension> oldVec)
	{
		if constexpr (std::is_same_v<NewValueType, OldValueType>)
			return oldVec;

		// TODO assert castability and complain

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
 			return new_type{
 				static_cast<NewValueType>(oldVec.x),
 				static_cast<NewValueType>(oldVec.y),
 				static_cast<NewValueType>(oldVec.z),
 				static_cast<NewValueType>(oldVec.w)
 			};

 		// TODO: implement more with loop
	}

	/*
	Cast to vector of different size.
	When shrinking, excess elements are discarded.
	When expanding, missing elements are initialized as per aggregate initialization rules.
	*/
	template <int NewDimension, typename ValueType, int OldDimension>
	constexpr auto dimension_cast(maths::VectorBase<ValueType, OldDimension> oldVec)
	{
		if constexpr (NewDimension == OldDimension)
			return oldVec;

		using new_type = maths::VectorBase<ValueType, NewDimension>;

		constexpr int smallestDimension = NewDimension < OldDimension ? NewDimension : OldDimension;

		if constexpr (smallestDimension == 2)
			return new_type {oldVec.x, oldVec.y};

		if constexpr (smallestDimension == 3)
			return new_type {oldVec.x, oldVec.y, oldVec.z};

		if constexpr (smallestDimension == 4)
			return new_type {oldVec.x, oldVec.y, oldVec.z, oldVec.w};

		// TODO: implement more with loop
	}
}	