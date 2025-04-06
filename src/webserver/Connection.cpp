#include "webserver/Connection.hpp"

Connection::Connection(int socketfd) : socket(socketfd), requestReady(false)
{
	socket.setNonBlocking();
}

Connection::~Connection() = default;

bool Connection::handleRead()
{
	std::string data = socket.receive();

	if (data.empty())
	{
		return false;
	}

	if (parser.parse(data))
	{
		requestReady = true;
	}

	return true;
}

bool Connection::isRequestReady() const
{
	return requestReady;
}

HttpRequest Connection::getRequest() const
{
	return parser.getRequest();
}

int Connection::getSocketFd() const
{
	return socket.getFileDescriptor();
}
