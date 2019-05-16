/*
Template MetaProgramming Library.

Contains constexpr helpers for templates.
*/

#pragma once

namespace ng::tmpl // lol, temple
{
	/*
	Is number of items in TArgs equal to Count.
	*/
	template <int Count, typename ... TArgs>
	inline constexpr bool countIsEqual = sizeof... (TArgs) == Count;

	/*
	Are all TArgs... of type T.
	references, const and volatile qualifiers are ignored wit std::decay.
	*/
	template<typename T, typename ... TArgs>
	inline constexpr bool areAllTypeof = (std::is_same_v<T, std::decay_t<TArgs>> && ...);
}