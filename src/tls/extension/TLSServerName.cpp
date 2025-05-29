#include "tls/extension/TLSServerName.hpp"

#include <cstdint>
#include <vector>

#include "networking/BufferReader.hpp"
#include "networking/BufferWriter.hpp"
#include "tls/extension/TLSExtension.hpp"

std::vector<uint8_t> TLSServerName::serialize() const
{
	BufferWriter buffer;

	buffer.writeUint16(static_cast<uint16_t>(TLSExtensionType::SERVER_NAME));
	buffer.writeUint16(static_cast<uint16_t>(name.size()));

	std::vector<uint8_t> data(name.begin(), name.end());
	buffer.writeBytes(data);

	return buffer.getBuffer();
}

TLSServerName parseTLSServerName(const std::vector<uint8_t>& data)
{
	BufferReader reader(data);
	TLSServerName serverName;

	reader.readUint16();

	while (!reader.complete())
	{
		auto type = static_cast<TLSServerNameType>(reader.readUint8());

		uint16_t length = reader.readUint16();
		auto content = reader.readBytes(length);

		if (type == TLSServerNameType::HOST_NAME)
		{
			serverName.nameType = type;
			serverName.name = std::string(content.begin(), content.end());
		}
	}

	return serverName;
}
