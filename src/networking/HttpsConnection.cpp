#include "networking/HttpsConnection.hpp"

#include "logging/Logger.hpp"
#include "tls/TLS.hpp"

HttpsConnection::HttpsConnection(int socketfd) : Connection(socketfd) {}
HttpsConnection::~HttpsConnection() = default;

bool HttpsConnection::handle()
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

Request HttpsConnection::getRequest() const
{
	return parser.getRequest();
}
