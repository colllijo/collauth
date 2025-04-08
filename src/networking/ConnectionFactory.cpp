#include "networking/ConnectionFactory.hpp"

#include "networking/HTTPConnection.hpp"
#include "networking/HTTPSConnection.hpp"

std::unique_ptr<Connection> ConnectionFactory::createConnection(int fd, Protocol protocol)
{
	switch (protocol)
	{
	case Protocol::HTTP:
		return std::make_unique<HTTPConnection>(fd);
	case Protocol::HTTPS:
		return std::make_unique<HTTPSConnection>(fd);
	default:
		throw std::invalid_argument("Invalid protocol");
	}
}
