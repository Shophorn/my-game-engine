#pragma once

#include "../debug.hpp"
#include "VectorBase.hpp"

namespace fmt
{
	template <typename ValueType, int Dimension>
	struct formatter<ng::maths::VectorBase<ValueType, Dimension>>
	{
		using vector_type = ng::maths::VectorBase<ValueType, Dimension>;

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
