#include "http/HttpRequest.hpp"

HttpRequest::HttpRequest() : method(HttpMethod::UNKNOWN), version(HttpVersion::UNKNOWN) {}

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

std::unordered_map<std::string, std::string> HttpRequest::getHeaders() const
{
	return headers;
}

std::optional<std::string> HttpRequest::getHeader(const std::string& name) const
{
	if (!headers.contains(name))
	{
		return std::nullopt;
	}

	return headers.at(name);
}

void HttpRequest::setHeader(const std::string& name, const std::string& value)
{
	headers[canonicalizeHeader(name)] = value;
}

std::string HttpRequest::getBody() const
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
