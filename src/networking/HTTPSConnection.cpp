#include "networking/HTTPSConnection.hpp"

#include "logging/Logger.hpp"
#include "tls/TLSContext.hpp"

HTTPSConnection::HTTPSConnection(int socketfd) : Connection(socketfd), context(socket) {}
HTTPSConnection::~HTTPSConnection() = default;

bool HTTPSConnection::handle()
{
	std::string data = socket.receive();

	if (data.empty())
	{
		return false;
	}

	if (!context.ready())
	{
		Logger::info("TLS handshake not complete.");
		return context.initialize(std::vector<uint8_t>(data.begin(), data.end()));
	}

	Logger::info("TLS handshake complete.");

	return false;
}

Request HTTPSConnection::getRequest() const
{
	return parser.getRequest();
}
