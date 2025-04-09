#include "tls/ServerHello.hpp"

#include "networking/BufferWriter.hpp"
#include "tls/TLS.hpp"

std::vector<uint8_t> ServerHello::serialize() const
{
	BufferWriter buffer;

	buffer.writeUint16(legacyVersion);
	buffer.writeBytes(random);
	buffer.writeBytes(sessionId);
	buffer.writeUint16(cipherSuite);
	buffer.writeUint8(compressionMethod);

	for (const auto& [type, data] : extensions)
	{
		buffer.writeUint16(static_cast<uint16_t>(type));
		buffer.writeUint16(data.size());
		buffer.writeBytes(data);
	}

	return buffer.getBuffer();
}

ServerHello buildServerHello(const ClientHello& clientHello)
{
	ServerHello serverHello;

	serverHello.legacyVersion = 0x0303;	 // TLS 1.2
	serverHello.random = {0x00};		 // Generate a cryptographically secure random value (32 bytes)
	serverHello.sessionId = clientHello.sessionId;
	serverHello.cipherSuite = 0x1301;	   // TLS_AES_128_GCM_SHA256
	serverHello.compressionMethod = 0x00;  // NULL compression

	serverHello.extensions = {{TLSExtensionType::SUPPORTED_VERSIONS, {0x03, 0x04}}};

	return serverHello;
}
