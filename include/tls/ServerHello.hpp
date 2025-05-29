#pragma once

#include "tls/ClientHello.hpp"
#include "tls/TLS.hpp"
#include "tls/extension/TLSExtension.hpp"
#include "tls/extension/TLSKeyShare.hpp"

// TODO: Change away from raw buffers
struct ServerHello
{
	uint16_t legacyVersion{};
	std::vector<uint8_t> random;
	std::vector<uint8_t> legacySessionIdEcho;
	TLSCipherSuite cipherSuite;
	uint8_t legacyCompressionMethod{};
	TLSExtension extensions;

	std::vector<uint8_t> serialize() const;
};

ServerHello buildServerHello(const ClientHello& clientHello);
ServerHello buildHelloRetry(const ClientHello& clientHello, TLSKeyShareType type, const std::vector<uint8_t>& keyShare);
