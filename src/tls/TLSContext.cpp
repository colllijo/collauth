#include "tls/TLSContext.hpp"

#include <unordered_map>

#include "logging/Logger.hpp"
#include "tls/ClientHello.hpp"
#include "tls/ServerHello.hpp"
#include "tls/TLS.hpp"
#include "tls/TLSKeyShare.hpp"
#include "tls/TLSPlaintext.hpp"
#include "tls/TLSServerName.hpp"
#include "tls/TLSSignature.hpp"
#include "tls/TLSVersion.hpp"

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
	if (!clientHello.extensions.contains(TLSExtensionType::SUPPORTED_VERSIONS))
	{
		Logger::error("TLS handshake failed: missing supported versions extension.");
		return false;
	};

	std::vector<TLSVersion> supportedVersions = parseTLSVersion(clientHello.extensions[TLSExtensionType::SUPPORTED_VERSIONS]);
	if (std::find(supportedVersions.begin(), supportedVersions.end(), TLSVersion::TLS_1_3) == supportedVersions.end())
	{
		Logger::info("TLS handshake failed: unsupported TLS version.");
		return false;
	}

	// Extension: Supported Groups
	if (clientHello.extensions.contains(TLSExtensionType::SUPPORTED_GROUPS))
	{
		std::vector<TLSKeyShareType> supportedGroups = parseTLSSupportedGroupes(clientHello.extensions[TLSExtensionType::SUPPORTED_GROUPS]);
	}

	// Extension: Key Share
	if (!clientHello.extensions.contains(TLSExtensionType::KEY_SHARE))
	{
		Logger::error("TLS handshake failed: missing key share extension.");
		return false;
	}

	parseTLSKeyShares(clientHello.extensions[TLSExtensionType::KEY_SHARE]);

	// TODO: Pick key share and compute shared key.

	// Extension: Signature Algorithms
	if (!clientHello.extensions.contains(TLSExtensionType::SIGNATURE_ALGORITHMS))
	{
		Logger::error("TLS handshake failed: missing signature algorithms extension.");
		return false;
	}

	std::vector<TLSSignatureScheme> signatureSchemes = parseTLSSignatureSchemes(clientHello.extensions[TLSExtensionType::SIGNATURE_ALGORITHMS]);

	// Extension: Server Name
	if (clientHello.extensions.contains(TLSExtensionType::SERVER_NAME))
	{
		std::unordered_map<TLSServerNameType, std::string> serverNames = parseTLSServerName(clientHello.extensions[TLSExtensionType::SERVER_NAME]);
		Logger::info("Server name: {}", serverNames[TLSServerNameType::HOST_NAME]);
	}

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
