#pragma once

#include <string>

enum class HTTPVersion
{
	HTTP_1_0,
	HTTP_1_1,
	HTTP_2_0,
	UNKNOWN
};

HTTPVersion parseHTTPVersion(const std::string& version);
