#pragma once

#include <cstdint>
#include <vector>

#include "tls/TLS.hpp"
#include "tls/extension/TLSExtension.hpp"

// TODO: Change away from raw buffers
struct ClientHello
{
	uint16_t legacyVersion{};
	std::vector<uint8_t> random;
	std::vector<uint8_t> sessionId;
	std::vector<TLSCipherSuite> cipherSuites;
	std::vector<uint8_t> compressionMethods;
	TLSExtension extensions;
};

ClientHello parseClientHello(const std::vector<uint8_t>& data);
