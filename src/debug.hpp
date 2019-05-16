/*
NG engine project
Leo Tamminen
*/

#pragma once

// standard library
#include <memory>
#include <string>
#include "LogChannel.hpp"

namespace ng
{
	namespace debug
	{
		inline LogChannel main;
		inline LogChannel physics;
		inline LogChannel rendering;
	
		// Easy log, smallest verbosity on main channel
		template <typename ... TArgs>
		void log(std::string format, TArgs ... args)
		{
			main.log(0, format, args...);
		}

		void initialize(const std::string & logDirectoryPath) noexcept;
		void terminate() noexcept;
	}
}