#include "http/HttpResponse.hpp"
#include "http/HttpStatus.hpp"

HttpResponse::HttpResponse(HttpStatus status) : status(status) {}

HttpStatus HttpResponse::getStatusCode() const
{
	return status;
}

HttpResponse& HttpResponse::setStatusCode(HttpStatus status)
{
	this->status = status;
	return *this;
}

std::optional<std::string> HttpResponse::getHeader(const std::string& name) const
{
	if (headers.contains(name))
	{
		return std::nullopt;
	}

	return headers.at(name);
}

HttpResponse& HttpResponse::setHeader(const std::string& name, const std::string& value)
{
	headers[name] = value;
	return *this;
}

std::string HttpResponse::getBody() const
{
	return body;
}

HttpResponse& HttpResponse::setBody(const std::string& body)
{
	this->body = body;
	return *this;
}

std::string HttpResponse::build() const
{
	std::string response = "HTTP/1.1 " + statusToString(status) + "\r\n";
	for (const auto& [key, value] : headers)
	{
		response += key + ": " + value + "\r\n";
	}

	response += "Content-Length: " + std::to_string(body.size()) + "\r\n";
	response += "\r\n";
	response += body;

	return response;
}
