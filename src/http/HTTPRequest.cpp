#include "http/HTTPRequest.hpp"

HTTPRequest::HTTPRequest() : method(HTTPMethod::UNKNOWN), version(HTTPVersion::UNKNOWN) {}

HTTPMethod HTTPRequest::getMethod() const
{
	return method;
}

std::string HTTPRequest::getPath() const
{
	return path;
}

HTTPVersion HTTPRequest::getVersion() const
{
	return version;
}

std::unordered_map<std::string, std::string> HTTPRequest::getHeaders() const
{
	return headers;
}

std::optional<std::string> HTTPRequest::getHeader(const std::string& name) const
{
	if (!headers.contains(name))
	{
		return std::nullopt;
	}

	return headers.at(name);
}

void HTTPRequest::setHeader(const std::string& name, const std::string& value)
{
	headers[canonicalizeHeader(name)] = value;
}

std::string HTTPRequest::getBody() const
{
	return body;
}

std::string canonicalizeHeader(const std::string& header)
{
	std::string result;
	bool capitalizeNext = true;

	for (char c : header)
	{
		if (c == '-')
		{
			capitalizeNext = true;
			result += c;
		}
		else if (capitalizeNext)
		{
			result += toupper(c);
			capitalizeNext = false;
		}
		else
		{
			result += tolower(c);
		}
	}

	return result;
}
