#pragma once

#include "http/HttpRequest.hpp"
class HttpResponse
{
public:
	static std::string generateResponse(const HttpRequest& request);
};
