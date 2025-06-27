#include "http/HTTPResponse.hpp"

#include <format>

#include "http/HTTPStatus.hpp"

HTTPResponse::HTTPResponse(HTTPStatus status) : status(status) {}

HTTPStatus HTTPResponse::getStatusCode() const
{
	return status;
}

HTTPResponse& HTTPResponse::setStatusCode(HTTPStatus status)
{
	this->status = status;
	return *this;
}

std::optional<std::string> HTTPResponse::getHeader(const std::string& name) const
{
	if (headers.contains(name))
	{
		return std::nullopt;
	}

	return headers.at(name);
}

HTTPResponse& HTTPResponse::setHeader(const std::string& name, const std::string& value)
{
	headers[name] = value;
	return *this;
}

std::string HTTPResponse::getBody() const
{
	return body;
}

HTTPResponse& HTTPResponse::setBody(const std::string& body)
{
	this->body = body;
	return *this;
}

std::string HTTPResponse::build() const
{
	std::string response = "HTTP/1.1 " + statusToString(status) + "\r\n";
	for (const auto& [key, value] : headers)
	{
		response += std::format("{}: {}\r\n", key, value);
	}

	response += "Content-Length: " + std::to_string(body.size()) + "\r\n";
	response += "\r\n";
	response += body;

	return response;
}
