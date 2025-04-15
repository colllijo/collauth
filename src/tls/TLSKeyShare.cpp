#include "tls/TLSKeyShare.hpp"

#include "networking/BufferReader.hpp"

std::vector<TLSKeyShareType> parseTLSSupportedGroupes(const std::vector<uint8_t>& data)
{
	BufferReader reader(data);
	std::vector<TLSKeyShareType> groups;

	// Length of the supported groups list
	reader.readUint16();

	while (!reader.complete())
	{
		TLSKeyShareType type = static_cast<TLSKeyShareType>(reader.readUint16());
		groups.push_back(type);
	}

	return groups;
}

std::unordered_map<TLSKeyShareType, std::vector<uint8_t>> parseTLSKeyShares(const std::vector<uint8_t>& data)
{
	BufferReader reader(data);
	std::unordered_map<TLSKeyShareType, std::vector<uint8_t>> keyShares;

	// Length of the key shares list
	reader.readUint16();

	while (!reader.complete())
	{
		TLSKeyShareType type = static_cast<TLSKeyShareType>(reader.readUint16());

		uint16_t length = reader.readUint16();
		keyShares[type] = reader.readBytes(length);
	}

	return keyShares;
}
