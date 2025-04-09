#pragma once

#include "tls/ClientHello.hpp"

struct ServerHello
{
	uint16_t legacyVersion;
	std::vector<uint8_t> random;
	std::vector<uint8_t> sessionId;
	uint16_t cipherSuite;
	uint8_t compressionMethod;
	std::unordered_map<TLSExtensionType, std::vector<uint8_t>> extensions;

	std::vector<uint8_t> serialize() const;
};

ServerHello buildServerHello(const ClientHello& clientHello);
