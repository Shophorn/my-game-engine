#pragma once

#include "../debug.hpp"

template<typename ValueType, int Dimensions>
struct vector_base
{
	static void test()
	{
		debug::log("hello from test vector");
	}

	using this_type = vector_base<ValueType, Dimensions>;

	this_type operator + (this_type other)
	{
		this_type result;


		return result;
	}
};

template <typename ValueType>
struct vector_2_base : vector_base<ValueType, 2>
{
	ValueType x;
	ValueType y;
};

using f2 = vector_2_base<float>;

void testVectors()
{
	f2::test();

	f2 a;
	f2 b;

	// f2 c = a + b;
}