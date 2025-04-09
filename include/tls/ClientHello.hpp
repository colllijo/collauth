#pragma once

#include <cstdint>
#include <unordered_map>
#include <vector>

#include "tls/TLS.hpp"

struct ClientHello
{
	uint16_t legacyVersion;
	std::vector<uint8_t> random;
	std::vector<uint8_t> sessionId;
	std::vector<TLSCipherSuite> cipherSuites;
	std::vector<uint8_t> compressionMethods;
	std::unordered_map<TLSExtensionType, std::vector<uint8_t>> extensions;
};

ClientHello parseClientHello(const std::vector<uint8_t>& data);
