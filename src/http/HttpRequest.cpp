#include "http/HttpRequest.hpp"

#include <iostream>
#include <sstream>

#include "http/HttpMethod.hpp"

HttpRequest::HttpRequest() : method(HttpMethod::UNKNOWN), version(HttpVersion::UNKNOWN) {}

void HttpRequest::reset()
{
	method = HttpMethod::UNKNOWN;
	path.clear();
	version = HttpVersion::UNKNOWN;
	headers.clear();
	body.clear();
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

	while (std::getline(iss, line) && !line.empty())
	{
		if (!parseHeaders(line))
		{
			std::cout << "Bad header: '" << line << "'" << std::endl;
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

bool HttpRequest::parseHeaders(const std::string& headersStr)
{
	std::istringstream iss(headersStr);
	std::string key, value;

	if (!(std::getline(iss, key, ':') && std::getline(iss, value)))
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
