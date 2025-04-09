#include "tls/ClientHello.hpp"

#include "networking/BufferReader.hpp"

ClientHello parseClientHello(const std::vector<uint8_t>& data)
{
	BufferReader buffer(data);
	ClientHello clientHello;

	clientHello.legacyVersion = buffer.readUint16();
	clientHello.random = buffer.readBytes(32);

	uint8_t sessionIdLength = buffer.readUint8();
	clientHello.sessionId = buffer.readBytes(sessionIdLength);

	uint16_t cipherSuitesLength = buffer.readUint16();
	for (size_t i = 0; i < cipherSuitesLength; i += 2)
	{
		clientHello.cipherSuites.push_back(static_cast<TLSCipherSuite>(buffer.readUint16()));
	}

	uint8_t compressionMethodsLength = buffer.readUint8();
	clientHello.compressionMethods = buffer.readBytes(compressionMethodsLength);

	uint16_t extensionsLength = buffer.readUint16();
	clientHello.extensions = parseTLSExtensions(buffer.readBytes(extensionsLength));

	return clientHello;
}
