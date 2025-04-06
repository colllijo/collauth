#pragma once

#include <optional>
#include <string>
#include <unordered_map>

#include "http/HttpMethod.hpp"
#include "http/HttpVersion.hpp"

class HttpRequest
{
public:
	HttpMethod method;
	std::string path;
	HttpVersion version;
	std::string body;

	HttpRequest();

	HttpMethod getMethod() const;

	std::string getPath() const;

	HttpVersion getVersion() const;

	std::unordered_map<std::string, std::string> getHeaders() const;

	std::optional<std::string> getHeader(const std::string& name) const;
	void setHeader(const std::string& name, const std::string& value);

	std::string getBody() const;

private:
	std::unordered_map<std::string, std::string> headers;
};

std::string canonicalizeHeader(const std::string& header);
