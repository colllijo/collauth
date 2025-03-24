#include "http/HttpResponse.hpp"

std::string HttpResponse::generateResponse(const HttpRequest&)
{
	std::string response;

	response += "HTTP/1.1 200 OK\r\n";
	response += "Content-Type: text/plain\r\n";
	response += "Connection: close\r\n";
	response += "Content-Length: 13\r\n";
	response += "\r\n";
	response += "Hello, World!";

	return response;
}
