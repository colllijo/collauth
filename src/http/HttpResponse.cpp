#include "http/HttpResponse.hpp"
#include "http/HttpStatus.hpp"

HttpResponse::HttpResponse(HttpStatus status) : status(status) {}

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
