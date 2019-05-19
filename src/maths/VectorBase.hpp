#pragma once

#include "../debug.hpp"
#include "../assertions.hpp"

template <typename ValueType, int Dimension>
struct VectorBase;

/*
TODO: we can implement this with std::array, but right now we only care about dimensions 2 - 4
because they are implemented separately
*/

/*
Debug formatting
*/	
namespace fmt
{
	template <typename ValueType, int Dimension>
	struct formatter<VectorBase<ValueType, Dimension>>
	{
		using vector_type = VectorBase<ValueType, Dimension>;

		UNUSED_FMT_PARSE

		template <typename FormatContext>
		auto format (const vector_type & vec, FormatContext & context)
		{
			if constexpr (Dimension == 1) 
				return format_to(context.begin(), "({})", vec.x);
			
			if constexpr (Dimension == 2)
				return format_to(context.begin(), "({}, {})", vec.x, vec.y);

			if constexpr (Dimension == 3)
				return format_to(context.begin(), "({}, {}, {})", vec.x, vec.y, vec.z);

			if constexpr (Dimension == 4)
				return format_to(context.begin(), "({}, {}, {}, {})", vec.x, vec.y, vec.z, vec.w);
		}
	};
}
