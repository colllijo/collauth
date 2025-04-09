#include "tls/TLSContext.hpp"

#include "logging/Logger.hpp"
#include "tls/ClientHello.hpp"
#include "tls/ServerHello.hpp"
#include "tls/TLS.hpp"
#include "tls/TLSPlaintext.hpp"

TLSContext::TLSContext() {}

bool TLSContext::initialize(const std::vector<uint8_t>& data)
{
	TLSPlaintext clientRecord = parseTLSPlaintext(data);

	if (clientRecord.type != TLSContentType::HANDSHAKE)
	{
		Logger::error("TLS handshake failed: not a handshake record.");
		return false;
	}

	auto [handshakeType, clientHandshake] = parseTLSHandshake(clientRecord.fragment);

	if (handshakeType != TLSHandshakeType::CLIENT_HELLO)
	{
		Logger::error("TLS handshake failed: not a ClientHello record.");
		return false;
	}

	ClientHello clientHello = parseClientHello(clientHandshake);
	ServerHello serverHello = buildServerHello(clientHello);

	auto serverHandshake = buildTLSHandshake(TLSHandshakeType::SERVER_HELLO, serverHello.serialize());

	TLSPlaintext serverRecord = buildTLSPlaintext(TLSContentType::HANDSHAKE, serverHandshake);

	return false;
}

bool TLSContext::ready() const
{
	return false;
}
