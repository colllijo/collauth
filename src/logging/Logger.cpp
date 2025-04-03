#include "logging/Logger.hpp"

#include <unistd.h>

#include <iomanip>
#include <regex>
#include <sstream>

std::mutex Logger::logMutex;

const char* RESET = "\033[0m";
const char* RED = "\033[31m";
const char* YELLOW = "\033[33m";
const char* GREEN = "\033[32m";
const char* CYAN = "\033[36m";
const char* BLUE = "\033[34m";
const char* MAGENTA = "\033[35m";
const char* BOLD = "\033[1m";

std::string Logger::getFunctionName(const std::source_location& location)
{
	std::regex functionNameRegex(R"((\w+::)*(\w+)\s*\()");

	std::string signature = location.function_name();
    std::smatch match;
    if (std::regex_search(signature, match, functionNameRegex)) {
        return match[2];
    }

	return signature;
}

std::string Logger::getRelativePath(const std::string& path)
{
	std::string marker = "/src/";

	size_t pos = path.find(marker);
	if (pos != std::string::npos)
	{
		return path.substr(pos + marker.length());
	}
	return path;
}

std::string Logger::getCurrentTime()
{
	auto now = std::chrono::system_clock::now();
	auto in_time_t = std::chrono::system_clock::to_time_t(now);

	std::ostringstream oss;
	oss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%dT%X%z");

	return oss.str();
}

bool isTerminal()
{
	return isatty(fileno(stdout));
}

std::string Logger::toString(Logger::Level level)
{
	if (isTerminal())
	{
		switch (level)
		{
		case Level::TRACE:
			return std::string(BLUE) + "TRACE" + RESET;
		case Level::DEBUG:
			return std::string(CYAN) + "DEBUG" + RESET;
		case Level::INFO:
			return std::string(GREEN) + "INFO" + RESET;
		case Level::WARNING:
			return std::string(YELLOW) + "WARNING" + RESET;
		case Level::ERROR:
			return std::string(RED) + "ERROR" + RESET;
		case Level::FATAL:
			return std::string(BOLD) + std::string(RED) + "FATAL" + RESET;
		}
	}
	else
	{
		switch (level)
		{
		case Level::TRACE:
			return "TRACE";
		case Level::DEBUG:
			return "DEBUG";
		case Level::INFO:
			return "INFO";
		case Level::WARNING:
			return "WARNING";
		case Level::ERROR:
			return "ERROR";
		case Level::FATAL:
			return "FATAL";
		}
	}
	return "[UNKNOWN]";
}
