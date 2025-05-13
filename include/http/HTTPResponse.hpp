#pragma once

#include <optional>
#include <string>
#include <unordered_map>

#include "http/HTTPStatus.hpp"

class HTTPResponse
{
public:
	HTTPStatus status;
	std::unordered_map<std::string, std::string> headers;
	std::string body;

	explicit HTTPResponse(HTTPStatus status = HTTPStatus::OK);

	HTTPStatus getStatusCode() const;
	HTTPResponse& setStatusCode(HTTPStatus status);

	std::optional<std::string> getHeader(const std::string& name) const;
	HTTPResponse& setHeader(const std::string& name, const std::string& value);

	std::string getBody() const;
	HTTPResponse& setBody(const std::string& body);

	std::string build() const;
};
