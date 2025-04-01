#include "http/HttpRequest.hpp"

#include <iostream>
#include <sstream>

HttpRequest::HttpRequest() : method(HttpMethod::UNKNOWN), version(HttpVersion::UNKNOWN) {}

void HttpRequest::reset()
{
	method = HttpMethod::UNKNOWN;
	path.clear();
	version = HttpVersion::UNKNOWN;
	headers.clear();
	body.clear();
}

HttpMethod HttpRequest::getMethod() const
{
	return method;
}

std::string HttpRequest::getPath() const
{
	return path;
}

HttpVersion HttpRequest::getVersion() const
{
	return version;
}

std::optional<std::string> HttpRequest::getHeader(const std::string& name) const
{
	if (!headers.contains(name))
	{
		return std::nullopt;
	}

	return headers.at(name);
}

std::unordered_map<std::string, std::string> HttpRequest::getHeaders() const
{
	return headers;
}

std::string HttpRequest::getBody() const
{
	return body;
}

bool HttpRequest::parse(const std::string& request)
{
	reset();

	std::istringstream iss(request);
	std::string line;

	if (!std::getline(iss, line) || !parseRequestLine(line))
	{
		std::cout << "Bad request line: " << line << std::endl;
		return false;
	}

	while (std::getline(iss, line) && line != "\r")
	{
		if (!parseHeader(line))
		{
			return false;
		}
	}

	if (headers.contains("Content-Length"))
	{
		int contentLength = std::stoi(headers["Content-Length"]);
		std::string buffer(contentLength, '\0');

		iss.read(buffer.data(), contentLength);

		parseBody(buffer);
	}

	return true;
}

bool HttpRequest::parseRequestLine(const std::string& requestLine)
{
	std::istringstream iss(requestLine);
	std::string methodStr, versionStr;

	if (!(iss >> methodStr >> path >> versionStr))
	{
		return false;
	}

	method = parseHttpMethod(methodStr);
	version = parseHttpVersion(versionStr);

	return method != HttpMethod::UNKNOWN && version != HttpVersion::UNKNOWN;
}

bool HttpRequest::parseHeader(const std::string& headerStr)
{
	size_t pos = headerStr.find(":");
	if (pos == std::string::npos || pos == 0 || pos == headerStr.size() - 1)
	{
		return false;
	}

	std::string key = headerStr.substr(0, pos);
	std::string value = headerStr.substr(pos + 1);

	key.erase(0, key.find_first_not_of(" \t"));
	key.erase(key.find_last_not_of(" \t") + 1);
	value.erase(0, value.find_first_not_of(" \t"));
	value.erase(value.find_last_not_of(" \t") + 1);

	if (key.empty() || value.empty())
	{
		return false;
	}

	headers[key] = value;
	return true;
}

void HttpRequest::parseBody(const std::string& bodyStr)
{
	body = bodyStr;
}
