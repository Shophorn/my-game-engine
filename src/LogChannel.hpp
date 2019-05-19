/*
NG engine project
Leo Tamminen
*/
#pragma once

#include <fstream>
#include <string>
#include <fmt/format.h>

#include <cmath>
#include <GLFW/glfw3.h>
	
namespace ng
{
	class LogChannel
	{
	public:
		int verbosityLevel = 0	;
		bool printToTerminal = true;
		std::string name;

		LogChannel () {}

		LogChannel (std::string channelName, std::string logFilePath)
			:	name (channelName),
				file (std::ofstream (logFilePath, std::ios::out)) {}

		/*
		Create log entry that will be printed on terminal also if verbosity is 
		set lover than current verbosity level. Text will be formatted using fmt-library.
		*/
		template <typename ... TArgs>
		void log(int verbosity, const std::string & text, TArgs ... args)
		{
			auto message = fmt::format(text, args ...);
			auto timeStr = timeStamp();

			/*
			Write to file. Format string inclues newline, but flush afterwards 
			so that we get most recent entry in case of crash.
			*/
			file << fmt::format(fileFormat, timeStr, message) << std::flush;

			bool doPrint = 
				printToTerminal
				&& verbosity <= verbosityLevel;

			if (doPrint)
			{
				fmt::print(terminalFormat, name, timeStr, message);
			}
		}

		/*
		Same as log, but with context
		*/
		template <typename ... TArgs>
		void error(std::string file, int line, const std::string & text, TArgs ... args)
		{
			auto message = fmt::format(text, args ...);
			auto output = fmt::format("file {}\nline {}:\n{}", file, line, message);
			auto timeStr = timeStamp ();

			/*
			Write to file. Format string inclues newline, but flush afterwards 
			so that we get most recent entry in case of crash.
			*/
			file << fmt::format(fileFormat, timeStr, output) << std::flush;			
			fmt::print(terminalFormat, name, timeStr, output);

		}

		/*
		Print directly to output file without creating any log happenings.
		*/
		template<typename ... TArgs>
		void print(std::string text, TArgs ... args)
		{
			file << fmt::format(text, args...) << std::endl;
		}

		/*
		Sets this channels terminal printing active or inactive.
		Entries will still be written to file.
		*/
		void setActive(bool value) noexcept 
		{ 
			printToTerminal = value;
		}

	private:
		std::ofstream file;

		inline static const std::string fileFormat = "[{0}]: {1}\n";
		inline static const std::string terminalFormat = "[{0} {1}]: {2}\n";

		/*
		Get formatted timestamp as we want to use it here
		*/
		static std::string timeStamp()
		{
			const static int secondsPrecision = 2;

			float seconds = glfwGetTime();
			int minutes = static_cast<int>(seconds) / 60;

			seconds = fmod(seconds, 60.0f);	

			return fmt::format("{0}:{1:.{2}f}", minutes, seconds, secondsPrecision);
		}
	};
}