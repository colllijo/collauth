#pragma once

#include <string>

enum class HTTPMethod
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
