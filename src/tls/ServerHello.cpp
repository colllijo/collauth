#include "tls/ServerHello.hpp"

#include "networking/BufferWriter.hpp"
#include "tls/ClientHello.hpp"
#include "tls/TLSVersion.hpp"

const std::vector<uint8_t> HELLO_RETRY_RANDOM = {0xCF, 0x21, 0xAD, 0x74, 0xE5, 0x9A, 0x61, 0x11, 0xBE, 0x1D, 0x8C, 0x02, 0x1E, 0x65, 0xB8, 0x91,
												 0xC2, 0xA2, 0x11, 0x16, 0x7A, 0xBB, 0x8C, 0x5E, 0x07, 0x9E, 0x09, 0xE2, 0xC8, 0xA8, 0x33, 0x9C};

std::vector<uint8_t> ServerHello::serialize() const
{
	BufferWriter buffer;

	buffer.writeUint16(legacyVersion);
	buffer.writeBytes(random);
	buffer.writeUint8(static_cast<uint8_t>(legacySessionIdEcho.size()));
	buffer.writeBytes(legacySessionIdEcho);
	buffer.writeUint16(static_cast<uint16_t>(cipherSuite));
	buffer.writeUint8(legacyCompressionMethod);

	auto extensionsData = extensions.serialize();
	buffer.writeUint16(static_cast<uint16_t>(extensionsData.size()));
	buffer.writeBytes(extensionsData);

	return buffer.getBuffer();
}

ServerHello buildServerHello(const ClientHello& /*unused*/)
{
	ServerHello serverHello;

	return serverHello;
}

ServerHello buildHelloRetry(const ClientHello& clientHello, TLSKeyShareType type, const std::vector<uint8_t>& keyShare)
{
	BufferWriter buffer;

	ServerHello serverHello;

	serverHello.legacyVersion = static_cast<uint16_t>(TLSVersion::TLS_1_2);
	serverHello.random = HELLO_RETRY_RANDOM;
	serverHello.legacySessionIdEcho = clientHello.sessionId;
	serverHello.cipherSuite = clientHello.cipherSuites.at(0);
	serverHello.legacyCompressionMethod = 0x00;

	serverHello.extensions.supportedVersions = {TLSVersion::TLS_1_3};
	serverHello.extensions.keyShares[type] = keyShare;

	return serverHello;
}
