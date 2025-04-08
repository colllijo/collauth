#include "networking/HTTPSConnection.hpp"

#include "logging/Logger.hpp"
#include "tls/TLS.hpp"

HTTPSConnection::HTTPSConnection(int socketfd) : Connection(socketfd) {}
HTTPSConnection::~HTTPSConnection() = default;

bool HTTPSConnection::handle()
{
	std::string data = socket.receive();

	if (data.empty()) return false;

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

Request HTTPSConnection::getRequest() const
{
	return parser.getRequest();
}
