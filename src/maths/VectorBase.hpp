
#pragma once

namespace ng::maths
{
	/*
	Template is undefined, since we specialize dimensions 2 - 4 to get named
	member variables.

	We could implement more than 4 dimension vectors with array and no named members
	here, as specializations would override this.
	*/
	template <typename ValueType, int Dimension>
	struct VectorBase;
}