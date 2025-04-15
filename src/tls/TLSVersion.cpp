#include "tls/TLSVersion.hpp"

#include "networking/BufferReader.hpp"

std::vector<TLSVersion> parseTLSVersion(const std::vector<uint8_t>& data)
{
	BufferReader reader(data);
	std::vector<TLSVersion> versions;

	size_t length = reader.readUint8();

	for (size_t i = 0; i < length / 2; ++i)
	{
		versions.push_back(static_cast<TLSVersion>(reader.readUint16()));
	}

	return versions;
}
