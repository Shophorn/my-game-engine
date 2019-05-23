
/*
Template Meta-Programming Library.

Contains constexpr helpers for templates.
*/

#pragma once

namespace ng::tmpl // lol, temple
{
	template<int A, int B>
	constexpr int min = A < B ? A : B;


	/*
	Is number of items in TArgs equal to Count.
	*/
	template <int Count, typename ... TArgs>
	constexpr bool countIsEqual = sizeof... (TArgs) == Count;

	/*
	Are all TArgs... of type T.
	references, const and volatile qualifiers are ignored wit std::decay.
	*/
	template<typename T, typename ... TArgs>
	constexpr bool areAllTypeof = (std::is_same_v<T, std::decay_t<TArgs>> && ...);


	namespace detail
	{
		template<typename T, size_t N, size_t... I>
		constexpr auto impl_arraySequence(std::index_sequence<I...>)
		{
			return std::array<T, N>{ {I...} };
		}
	}

	/*
	Create array containing sequence of numbers.
	Source: somewhere from stackoverflow. There was hints also to add lambda into this.
	*/
	template<typename T, size_t N>
	constexpr auto arraySequence()
	{
		  return detail::impl_arraySequence<T, N>(std::make_index_sequence<N>{});
	}
}