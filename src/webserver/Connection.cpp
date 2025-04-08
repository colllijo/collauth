#include "webserver/Connection.hpp"

#include "logging/Logger.hpp"
#include "tls/TLS.hpp"

Connection::Connection(int socketfd, bool tls) : socket(socketfd), isTls(tls), requestReady(false)
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

	if (isTls)
	{
		ClientHello clientHello;

		if (parseClientHello(std::vector<uint8_t>(data.begin(), data.end()), clientHello))
		{
			Logger::info("Successfully parsed client hello.");
		}
		else
		{
			Logger::error("Error parsing client hello.");
		}

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
