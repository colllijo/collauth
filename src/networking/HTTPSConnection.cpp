#include "networking/HTTPSConnection.hpp"

#include "logging/Logger.hpp"

HTTPSConnection::HTTPSConnection(int socketfd) : Connection(socketfd) {}
HTTPSConnection::~HTTPSConnection() = default;

bool HTTPSConnection::handle()
{
	std::string data = socket.receive();

	if (data.empty()) return false;

	if (!context.ready())
	{
		Logger::info("TLS handshake not complete.");
		context.initialize(std::vector<uint8_t>(data.begin(), data.end()));

		return false;
	}

	Logger::info("TLS handshake complete.");

	return false;
}

Request HTTPSConnection::getRequest() const
{
	return parser.getRequest();
}
