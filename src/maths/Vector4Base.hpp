#pragma once

#include "VectorBase.hpp"
#include "../assertions.hpp"

namespace ng::maths
{
	/*
	Base alias for 4d vector types
	*/
	template<typename T>
	using Vector4Base = VectorBase<T, 4>;

	template<typename ValueType>
	struct VectorBase<ValueType, 4>
	{
		constexpr static int dimension = 4;
		
		using value_type = ValueType;
		using this_type = VectorBase<value_type, dimension>;

		union { value_type x, r; };
		union { value_type y, g; };
		union { value_type z, b; };
		union { value_type w, a; };

		// No initialize constructor
		constexpr VectorBase() = default;

		explicit constexpr VectorBase (value_type v)
			: x(v), y(v), z(v) {}

		constexpr VectorBase(value_type x, value_type y, value_type z)
			: x(x), y(y), z(z) {}


		value_type & operator [] (int index)
		{
			NG_ASSERT (0 <= index && index < dimension);

			/*
			Writing first entry as default supresses no return warning,
			since we already asserted that index is valid.
			*/
			switch (index)
			{
				default: return x;
				case 1: return y;
				case 2: return z;
				case 3: return w;
			}
		}

		value_type operator [] (int index) const
		{
			NG_ASSERT (0 <= index && index < dimension);

			switch (index)
			{
				default: return x;
				case 1: return y;
				case 2: return z;
				case 3: return w;
			}
		}

		const value_type * valuePtr() const
		{
			return reinterpret_cast<const value_type*>(this);
		}		
	};
}