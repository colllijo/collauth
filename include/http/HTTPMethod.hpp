#pragma once

#include <cstdint>
#include <string>

enum class HTTPMethod : uint8_t
{
	GET,
	POST,
	PUT,
	DELETE,
	HEAD,
	OPTIONS,
	PATCH,
	UNKNOWN
};

HTTPMethod parseHTTPMethod(const std::string& method);
std::string httpMethodToString(HTTPMethod method);
