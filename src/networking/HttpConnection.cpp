#include "networking/HttpConnection.hpp"

HttpConnection::HttpConnection(int socketfd) : Connection(socketfd) {}
HttpConnection::~HttpConnection() = default;

bool HttpConnection::handle()
{
	std::string data = socket.receive();

	if (data.empty()) return false;

	if (parser.parse(data))
	{
		requestReady = true;
	}

	return true;
}

Request HttpConnection::getRequest() const
{
	return parser.getRequest();
}
