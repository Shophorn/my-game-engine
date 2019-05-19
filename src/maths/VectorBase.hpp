#pragma once

namespace ng::maths
{
	/*
	Template is undefined, since we need to specialize to each size.
	We could implement more than 4 dimension vectors with std::array and no named members
	here, as specializations would override this.
	*/
	template <typename ValueType, int Dimension>
	struct VectorBase;
}