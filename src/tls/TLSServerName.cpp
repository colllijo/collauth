#include "tls/TLSServerName.hpp"

#include <vector>

#include "networking/BufferReader.hpp"

std::unordered_map<TLSServerNameType, std::string> parseTLSServerName(const std::vector<uint8_t>& data)
{
	BufferReader reader(data);
	std::unordered_map<TLSServerNameType, std::string> serverNames;

	// Length of the server names list
	reader.readUint16();

	while (!reader.complete())
	{
		auto type = static_cast<TLSServerNameType>(reader.readUint8());

		uint16_t length = reader.readUint16();
		std::vector<uint8_t> content = reader.readBytes(length);

		serverNames[type] = std::string(content.begin(), content.end());
	}

	return serverNames;
}
