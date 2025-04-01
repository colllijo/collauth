#pragma once

#include <optional>
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

	HttpStatus getStatusCode() const;
	HttpResponse& setStatusCode(HttpStatus status);

	std::optional<std::string> getHeader(const std::string& name) const;
	HttpResponse& setHeader(const std::string& name, const std::string& value);

	std::string getBody() const;
	HttpResponse& setBody(const std::string& body);

	std::string build() const;
};
