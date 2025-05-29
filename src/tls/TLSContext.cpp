#include "tls/TLSContext.hpp"

#include <algorithm>
#include <atomic>
#include <unordered_map>

#include "cryptography/Random.hpp"
#include "logging/Logger.hpp"
#include "math/Number.hpp"
#include "tls/ClientHello.hpp"
#include "tls/ServerHello.hpp"
#include "tls/TLS.hpp"
#include "tls/TLSPlaintext.hpp"
#include "tls/TLSSignature.hpp"
#include "tls/TLSVersion.hpp"
#include "tls/extension/TLSKeyShare.hpp"
#include "tls/extension/TLSServerName.hpp"

TLSContext::TLSContext(Socket& socket) : socket(socket), state(TLSState::INITIAL) {}

bool TLSContext::initialize(const std::vector<uint8_t>& data)
{
	Logger::info("Initializing TLS context. ({})", static_cast<int>(state));
	switch (state)
	{
	case TLSState::INITIAL:
		return exchangeHandshake(data);
	case TLSState::NEGOTIATING:
		Logger::info("TLS handshake in progress.");
		return false;
	default:
		return false;
	}
}

bool TLSContext::exchangeHandshake(const std::vector<uint8_t>& data)
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

	// Extension: Supported Versions
	if (std::find(clientHello.extensions.supportedVersions.begin(), clientHello.extensions.supportedVersions.end(), TLSVersion::TLS_1_3) ==
		clientHello.extensions.supportedVersions.end())
	{
		Logger::info("TLS handshake failed: unsupported TLS version.");
		return false;
	}

	for (const auto& type : clientHello.extensions.supportedGroups)
	{
		Logger::debug("{:04X}", static_cast<uint16_t>(type));
	}

	// TODO: Check that server supports or accepts at least of the groups supported by the server
	// else a handshake_failure or insufficient_security answer needs to be sent.

	// TODO: Choose a cipher suite and a group

	// TODO: Check if the keyshare for the choosen group is contained
	// else send a HelloRetryRequest

	// TODO: Pick key share and compute shared key.

	// Extension: Signature Algorithms
	// TODO: Parse TLS Signature algorithms

	// Extension: Server Name
	Logger::info("Server name: {}", clientHello.extensions.serverName.name);

	// TODO: Acutally do this thing correctly
	ServerHello serverHello = buildServerHello(clientHello);

	auto serverHandshake = buildTLSHandshake(TLSHandshakeType::SERVER_HELLO, serverHello.serialize());

	TLSPlaintext serverRecord = buildTLSPlaintext(TLSContentType::HANDSHAKE, serverHandshake);

	auto res = serverRecord.serialize();

	socket.send(res);

	state = TLSState::NEGOTIATING;

	serverRecord = buildTLSPlaintext(TLSContentType::CHANGE_CIPHER_SPEC, {0x01});
	socket.send(serverRecord.serialize());

	return true;
}

bool TLSContext::ready() const
{
	return false;
}
