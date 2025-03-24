#pragma once

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
	std::unordered_map<std::string, std::string> headers;
	std::string body;

	HttpRequest();

	void reset();
	bool parse(const std::string& request);
	bool parseRequestLine(const std::string& requestLine);
	bool parseHeaders(const std::string& headers);
	void parseBody(const std::string& body);

private:
};
