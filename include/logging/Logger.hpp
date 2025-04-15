#pragma once

#include <format>
#include <iostream>
#include <mutex>
#include <source_location>
#include <string>
#include <vector>

class Logger
{
public:
	enum class Level
	{
		TRACE,
		DEBUG,
		INFO,
		WARNING,
		ERROR,
		FATAL
	};

	template <typename... Args>
	struct trace
	{
		trace(const std::string& message, Args&&... args, const std::source_location& location = std::source_location::current())
		{
			std::lock_guard<std::mutex> lock(logMutex);

			std::string logMessage = std::format("[{}] [{}] [{}:{} ({})]: {}", getCurrentTime(), toString(Level::TRACE), getRelativePath(location.file_name()), location.line(),
												 getFunctionName(location), std::vformat(message, std::make_format_args(args...)));

			std::cout << logMessage << "\n";
		}
	};

	template <typename... Args>
	trace(const std::string& message, Args&&...) -> trace<Args...>;

	template <typename... Args>
	struct debug
	{
		debug(const std::string& message, Args&&... args, const std::source_location& location = std::source_location::current())
		{
			std::lock_guard<std::mutex> lock(logMutex);

			std::string logMessage = std::format("[{}] [{}] [{}:{} ({})]: {}", getCurrentTime(), toString(Level::DEBUG), getRelativePath(location.file_name()), location.line(),
												 getFunctionName(location), std::vformat(message, std::make_format_args(args...)));

			std::cout << logMessage << "\n";
		}
	};

	template <typename... Args>
	debug(const std::string& message, Args&&...) -> debug<Args...>;

	template <typename... Args>
	struct info
	{
		info(const std::string& message, Args&&... args, const std::source_location& location = std::source_location::current())
		{
			std::lock_guard<std::mutex> lock(logMutex);

			std::string logMessage = std::format("[{}] [{}] [{}:{} ({})]: {}", getCurrentTime(), toString(Level::INFO), getRelativePath(location.file_name()), location.line(),
												 getFunctionName(location), std::vformat(message, std::make_format_args(args...)));

			std::cout << logMessage << "\n";
		}
	};

	template <typename... Args>
	info(const std::string& message, Args&&...) -> info<Args...>;

	template <typename... Args>
	struct warning
	{
		warning(const std::string& message, Args&&... args, const std::source_location& location = std::source_location::current())
		{
			std::lock_guard<std::mutex> lock(logMutex);

			std::string logMessage = std::format("[{}] [{}] [{}:{} ({})]: {}", getCurrentTime(), toString(Level::WARNING), getRelativePath(location.file_name()), location.line(),
												 getFunctionName(location), std::vformat(message, std::make_format_args(args...)));

			std::cerr << logMessage << "\n";
		}
	};

	template <typename... Args>
	warning(const std::string& message, Args&&...) -> warning<Args...>;

	template <typename... Args>
	struct error
	{
		error(const std::string& message, Args&&... args, const std::source_location& location = std::source_location::current())
		{
			std::lock_guard<std::mutex> lock(logMutex);

			std::string logMessage = std::format("[{}] [{}] [{}:{} ({})]: {}", getCurrentTime(), toString(Level::ERROR), getRelativePath(location.file_name()), location.line(),
												 getFunctionName(location), std::vformat(message, std::make_format_args(args...)));

			std::cerr << logMessage << "\n";
		}
	};

	template <typename... Args>
	error(const std::string& message, Args&&...) -> error<Args...>;

	template <typename... Args>
	struct fatal
	{
		fatal(const std::string& message, Args&&... args, const std::source_location& location = std::source_location::current())
		{
			std::lock_guard<std::mutex> lock(logMutex);

			std::string logMessage = std::format("[{}] [{}] [{}:{} ({})]: {}", getCurrentTime(), toString(Level::FATAL), getRelativePath(location.file_name()), location.line(),
												 getFunctionName(location), std::vformat(message, std::make_format_args(args...)));

			std::cerr << logMessage << "\n";
		}
	};

	template <typename... Args>
	fatal(const std::string& message, Args&&...) -> fatal<Args...>;

	static std::string toHex(const std::vector<uint8_t>& data);

private:
	static std::mutex logMutex;

	static std::string getFunctionName(const std::source_location& location);
	static std::string getRelativePath(const std::string& path);
	static std::string getCurrentTime();
	static std::string toString(Level level);
};
