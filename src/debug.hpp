/*
NG engine project
Leo Tamminen
*/

#pragma once

// standard library
#include <memory>
#include <string>

// This is included for convenience. Bad habit though.
#include <sstream> 

#include "LogChannel.hpp"

#include <fmt/format.h>
/*
Stupid multiline macro. Escape slashes **MUST** be last characters here to
split macro on multiple lines. No invisible whitespace allowed!!
*/ 
#define UNUSED_FMT_PARSE 							\
		template <typename ParseContext> 			\
		constexpr auto parse (ParseContext &ctx) 	\
		{ return ctx.begin(); }

namespace ng
{
	namespace debug
	{
		inline LogChannel main;
		inline LogChannel physics;
		inline LogChannel rendering;
	
		// Easy log, smallest verbosity on main channel
		// :TODO: make forwarding stuff
		template <typename ... TArgs>
		void log(std::string format, const TArgs ... args)
		{
			main.log(0, format, args...);
		}

		void initialize(const std::string & logDirectoryPath) noexcept;
		void terminate() noexcept;
	}
}