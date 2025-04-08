#pragma once

#include <cstdint>
#include <vector>

#include "logging/Logger.hpp"
#include "networking/ByteBuffer.hpp"

// https://tls13.xargs.org/#client-hello/annotated
// https://www.rfc-editor.org/rfc/rfc8446#section-4.1.2
struct RecordHeader
{
	uint8_t type;
	uint16_t version;
	uint16_t length;
};

struct HandshakeHeader
{
	uint8_t type;
	uint32_t length;
};

struct ClientHello
{
	uint16_t version;
	std::vector<uint8_t> random;
	std::vector<uint8_t> sessionID;
	std::vector<uint8_t> cipherSuites;
	std::vector<uint8_t> compressionMethods;
	std::vector<uint8_t> extensions;  // Extensions
};

bool parseClientHello(const std::vector<uint8_t>& data, ClientHello clientHello)
{
	ByteBuffer buffer(data);

	RecordHeader record;
	record.type = buffer.readUint8();
	record.version = buffer.readUint16();
	record.length = buffer.readUint16();

	Logger::info("RecordHeader:\n\tType: 0x{:02X}\n\tVersion: 0x{:04X}\n\tLength: 0x{:04X}", record.type, record.version, record.length);

	HandshakeHeader handshake;
	uint32_t value = buffer.readUint32();
	handshake.type = value >> 24;
	handshake.length = value & 0xFFFFFF;

	Logger::info("HandshakeHeader:\n\tType: 0x{:02X}\n\tLength: 0x{:04X}", handshake.type, handshake.length);

	clientHello.version = buffer.readUint16();

	Logger::info("TLS version: 0x{:04X}", clientHello.version);

	clientHello.random = buffer.readBytes(32);

	uint8_t sessionIdLength = buffer.readUint8();
	clientHello.sessionID = buffer.readBytes(sessionIdLength);

	uint16_t cipherSuiteLength = buffer.readUint16();
	clientHello.cipherSuites = buffer.readBytes(cipherSuiteLength);

	Logger::info("Available cipher suites: 0x{:04X}", cipherSuiteLength);
	for (size_t i = 0; i < (cipherSuiteLength / 2); i++)
	{
		Logger::info("\t0x{:04X}", clientHello.cipherSuites[i * 2] << 8 | clientHello.cipherSuites[i * 2 + 1]);
	}

	uint8_t compressionMethodLength = buffer.readUint8();
	clientHello.compressionMethods = buffer.readBytes(compressionMethodLength);

	Logger::info("Available compression methods: 0x{:04X}", compressionMethodLength);
	for (size_t i = 0; i < compressionMethodLength; i++)
	{
		Logger::info("\t0x{:02X}", clientHello.compressionMethods[i]);
	}

	uint16_t extensionLength = buffer.readUint16();
	clientHello.extensions = buffer.readBytes(extensionLength);

	return true;
}
