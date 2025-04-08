#include "networking/Connection.hpp"

Connection::Connection(int socketfd) : socket(socketfd), requestReady(false)
{
	socket.setNonBlocking();
}

bool Connection::isRequestReady() const
{
	return requestReady;
}

int Connection::getSocketFd() const
{
	return socket.getFileDescriptor();
}
