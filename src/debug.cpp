#include <string>
#include <filesystem>
#include <system_error>
#include <fmt/format.h>

#include "debug.hpp"
#include "LogChannel.hpp"

namespace
{
	constexpr const char * removeFirstNewLine(const char * source)
	{
		return source + 1;
	}

	constexpr const char * sharedHeader = removeFirstNewLine(R"(
NG Engine Project
Created by Leo Tamminen, rights reversed.
	)");

	constexpr const char * mainLogHeader = removeFirstNewLine(R"(
MAIN Channel log file
	)");

	/*
	Happy convenience class to create channels
	*/
	struct channelCreator
	{
		std::string directoryPath;

		channelCreator (const std::string & directoryPath)
			: directoryPath (directoryPath) {}

		ng::LogChannel create (
			const std::string & channelName,
			const std::string & fileName,
			const std::string & header
		){
			ng::LogChannel channel (channelName, directoryPath + fileName);
			channel.print(sharedHeader);
			channel.print(header);
			return channel;
		}
	};
}

void ng::debug::initialize(const std::string & logDirectoryPath) noexcept
{
	// Use non-throwing overload for create_directories.
	std::error_code errorCode;
	bool directoryCreated 
		= std::filesystem::create_directories(logDirectoryPath, errorCode);

	channelCreator creator (logDirectoryPath);

	main = creator.create("MAIN", "main.log", mainLogHeader);
	physics = creator.create("PHYSICS", "physics.log", "PHYSIX log");
}