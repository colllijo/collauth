#pragma once

#include <string>

enum class HttpVersion
{
	HTTP_1_0,
	HTTP_1_1,
	HTTP_2_0,
	UNKNOWN
};

HttpVersion parseHttpVersion(const std::string& version);
