#include "tls/ServerHello.hpp"

#include "cryptography/Random.hpp"
#include "networking/BufferWriter.hpp"
#include "tls/TLS.hpp"

std::vector<uint8_t> ServerHello::serialize() const
{
	BufferWriter buffer;

	buffer.writeUint16(legacyVersion);
	buffer.writeBytes(random);
	buffer.writeUint8(static_cast<uint8_t>(sessionId.size()));
	buffer.writeBytes(sessionId);
	buffer.writeUint16(static_cast<uint16_t>(cipherSuite));
	buffer.writeUint8(compressionMethod);

	BufferWriter extensionBuffer;
	for (const auto& [type, data] : extensions)
	{
		extensionBuffer.writeUint16(static_cast<uint16_t>(type));
		extensionBuffer.writeUint16(data.size());
		extensionBuffer.writeBytes(data);
	}

	buffer.writeUint16(static_cast<uint16_t>(extensionBuffer.getBuffer().size()));
	buffer.writeBytes(extensionBuffer.getBuffer());

	return buffer.getBuffer();
}

ServerHello buildServerHello(const ClientHello& clientHello)
{
	ServerHello serverHello;

	serverHello.legacyVersion = 0x0303;	 // TLS 1.2
	serverHello.random = generateRandomBytes(32);
	serverHello.sessionId = clientHello.sessionId;
	serverHello.cipherSuite = TLSCipherSuite::TLS_AES_128_GCM_SHA256;
	serverHello.compressionMethod = 0x00;  // NULL compression

	serverHello.extensions = {
		{TLSExtensionType::SUPPORTED_VERSIONS, {0x03, 0x04}},
		{TLSExtensionType::KEY_SHARE, {0x00, 0x1d, 0x00, 0x20, 0x9f, 0xd7, 0xad, 0x6d, 0xcf, 0xf4, 0x29, 0x8d, 0xd3, 0xf9, 0x6d, 0x5b, 0x1b, 0x2a,
									   0xf9, 0x10, 0xa0, 0x53, 0x5b, 0x14, 0x88, 0xd7, 0xf8, 0xfa, 0xbb, 0x34, 0x9a, 0x98, 0x28, 0x80, 0xb6, 0x15}},
	};

	return serverHello;
}
