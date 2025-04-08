#include "networking/ConnectionFactory.hpp"

#include "networking/HttpConnection.hpp"
#include "networking/HttpsConnection.hpp"

std::unique_ptr<Connection> ConnectionFactory::createConnection(int fd, Protocol protocol)
{
	switch (protocol)
	{
	case Protocol::HTTP:
		return std::make_unique<HttpConnection>(fd);
	case Protocol::HTTPS:
		return std::make_unique<HttpsConnection>(fd);
	default:
		throw std::invalid_argument("Invalid protocol");
	}
}
