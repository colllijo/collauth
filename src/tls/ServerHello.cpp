#include "tls/ServerHello.hpp"

#include "networking/BufferWriter.hpp"

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

ServerHello buildServerHello(const ClientHello& /*unused*/)
{
	ServerHello serverHello;

	return serverHello;
}
