#pragma once

template<typename T>
using Vector2Base = VectorBase<T, 2>;


template<typename ValueType>
struct VectorBase<ValueType, 2>
{
	constexpr static int dimension = 2;
	
	using value_type = ValueType;
	using this_type = VectorBase<value_type, dimension>;

	union { value_type x, r; };
	union { value_type y, g; };

	// No initialize constructor
	constexpr VectorBase() = default;

	explicit constexpr VectorBase (value_type v)
		: x(v), y(v) {}

	constexpr VectorBase(value_type x, value_type y)
		: x(x), y(y) {}


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
		}
	}

	value_type operator [] (int index) const
	{
		NG_ASSERT (0 <= index && index < dimension);

		switch (index)
		{
			default: return x;
			case 1: return y;
		}
	}

	const value_type * valuePtr() const
	{
		return reinterpret_cast<const value_type*>(this);
	}
};

