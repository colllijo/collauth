#include "networking/HTTPConnection.hpp"

HTTPConnection::HTTPConnection(int socketfd) : Connection(socketfd) {}
HTTPConnection::~HTTPConnection() = default;

bool HTTPConnection::handle()
{
	std::string data = socket.receive();

	if (data.empty()) return false;

	if (parser.parse(data))
	{
		requestReady = true;
	}

	return true;
}

Request HTTPConnection::getRequest() const
{
	return parser.getRequest();
}
