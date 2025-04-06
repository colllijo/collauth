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
