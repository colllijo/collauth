#include "tls/TLSPlaintext.hpp"

#include <vector>

#include "networking/BufferReader.hpp"
#include "networking/BufferWriter.hpp"

std::vector<uint8_t> TLSPlaintext::serialize() const
{
	BufferWriter buffer;

	buffer.writeUint8(static_cast<uint8_t>(type));
	buffer.writeUint16(version);
	buffer.writeUint16(fragment.size());
	buffer.writeBytes(fragment);

	return buffer.getBuffer();
}

TLSPlaintext parseTLSPlaintext(const std::vector<uint8_t>& data)
{
	BufferReader buffer(data);
	TLSPlaintext record;

	record.type = static_cast<TLSContentType>(buffer.readUint8());
	record.version = buffer.readUint16();

	uint16_t fragmentLength = buffer.readUint16();
	record.fragment = buffer.readBytes(fragmentLength);

	return record;
}

TLSPlaintext buildTLSPlaintext(TLSContentType type, const std::vector<uint8_t>& fragment)
{
	TLSPlaintext record;

	record.type = type;
	record.version = 0x0303;  // TLS 1.2
	record.fragment = fragment;

	return record;
}
