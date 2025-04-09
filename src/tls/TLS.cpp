#include "tls/TLS.hpp"

#include <unordered_map>

#include "networking/BufferReader.hpp"
#include "networking/BufferWriter.hpp"

std::tuple<TLSHandshakeType, std::vector<uint8_t>> parseTLSHandshake(const std::vector<uint8_t>& data)
{
	BufferReader buffer(data);
	TLSHandshakeHeader header;
	std::vector<uint8_t> handshake;

	header.handshakeType = static_cast<TLSHandshakeType>(buffer.readUint8());
	header.length = (buffer.readUint8() << 16) | (buffer.readUint8() << 8) | buffer.readUint8();

	handshake = buffer.readBytes(header.length);

	return {header.handshakeType, handshake};
}

std::vector<uint8_t> buildTLSHandshake(TLSHandshakeType handshakeType, const std::vector<uint8_t>& handshake)
{
	BufferWriter buffer;

	buffer.writeUint8(static_cast<uint8_t>(handshakeType));
	buffer.writeUint24(handshake.size());
	buffer.writeBytes(handshake);

	return buffer.getBuffer();
}

std::unordered_map<TLSExtensionType, std::vector<uint8_t>> parseTLSExtensions(const std::vector<uint8_t>& data)
{
	BufferReader buffer(data);
	std::unordered_map<TLSExtensionType, std::vector<uint8_t>> extensions;

	while (!buffer.complete())
	{
		TLSExtensionType type = static_cast<TLSExtensionType>(buffer.readUint16());

		uint16_t length = buffer.readUint16();
		extensions[type] = buffer.readBytes(length);
	}

	return extensions;
}
