#pragma once

#include <optional>
#include <string>
#include <unordered_map>

#include "http/HTTPMethod.hpp"
#include "http/HTTPVersion.hpp"

class HTTPRequest
{
public:
	HTTPMethod method;
	std::string path;
	HTTPVersion version;
	std::string body;

	HTTPRequest();

	HTTPMethod getMethod() const;

	std::string getPath() const;

	HTTPVersion getVersion() const;

	std::unordered_map<std::string, std::string> getHeaders() const;

	std::optional<std::string> getHeader(const std::string& name) const;
	void setHeader(const std::string& name, const std::string& value);

	std::string getBody() const;

private:
	std::unordered_map<std::string, std::string> headers;
};

std::string canonicalizeHeader(const std::string& header);
