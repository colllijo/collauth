#pragma once

#include <string>
#include <unordered_map>

#include "http/HttpStatus.hpp"

class HttpResponse
{
public:
	HttpStatus status;
	std::unordered_map<std::string, std::string> headers;
	std::string body;

	HttpResponse(HttpStatus status = HttpStatus::OK);

	std::string build() const;
};
