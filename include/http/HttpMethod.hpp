#pragma once

#include <string>

enum class HttpMethod
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

HttpMethod parseHttpMethod(const std::string& method);
