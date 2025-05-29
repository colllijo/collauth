#include "tls/TLS.hpp"

#include "networking/BufferReader.hpp"
#include "networking/BufferWriter.hpp"

std::tuple<TLSHandshakeType, std::vector<uint8_t>> parseTLSHandshake(const std::vector<uint8_t>& data)
{
	BufferReader buffer(data);
	TLSHandshakeHeader header{};
	std::vector<uint8_t> handshake;

	header.handshakeType = static_cast<TLSHandshakeType>(buffer.readUint8());
	header.length = static_cast<uint32_t>(buffer.readUint8() << 16ULL) | static_cast<uint32_t>(buffer.readUint8() << 8ULL) | buffer.readUint8();

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
