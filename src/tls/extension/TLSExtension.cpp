#include "tls/extension/TLSExtension.hpp"

#include "logging/Logger.hpp"
#include "networking/BufferReader.hpp"
#include "networking/BufferWriter.hpp"
#include "tls/TLSVersion.hpp"
#include "tls/extension/TLSKeyShare.hpp"

std::vector<uint8_t> TLSExtension::serialize() const
{
	BufferWriter buffer;

	// Server Name
	buffer.writeBytes(serverName.serialize());

	// Supported Versions
	buffer.writeUint16(static_cast<uint16_t>(TLSExtensionType::SUPPORTED_VERSIONS));
	buffer.writeUint16(static_cast<uint16_t>(supportedVersions.size() * 2));
	for (const auto& version : supportedVersions) buffer.writeUint16(static_cast<uint16_t>(version));

	// Key Shares
	BufferWriter keySharesBuffer;

	for (const auto& [type, data] : keyShares)
	{
		keySharesBuffer.writeUint16(static_cast<uint16_t>(type));
		keySharesBuffer.writeUint16(static_cast<uint16_t>(data.size()));
		keySharesBuffer.writeBytes(data);
	}

	buffer.writeUint16(static_cast<uint16_t>(TLSExtensionType::KEY_SHARE));
	buffer.writeUint16(static_cast<uint16_t>(keySharesBuffer.getBuffer().size()));
	buffer.writeBytes(keySharesBuffer.getBuffer());

	return buffer.getBuffer();
}

TLSExtension parseTLSExtensions(const std::vector<uint8_t>& data)
{
	BufferReader buffer(data);
	TLSExtension extension;

	while (!buffer.complete())
	{
		auto type = static_cast<TLSExtensionType>(buffer.readUint16());

		uint16_t length = buffer.readUint16();
		auto data = buffer.readBytes(length);

		switch (type)
		{
		case TLSExtensionType::SERVER_NAME:
			extension.serverName = parseTLSServerName(data);
			break;
		case TLSExtensionType::SUPPORTED_VERSIONS:
			extension.supportedVersions = parseTLSVersion(data);
			break;
		case TLSExtensionType::SUPPORTED_GROUPS:
			extension.supportedGroups = parseTLSSupportedGroupes(data);
			break;
		case TLSExtensionType::SIGNATURE_ALGORITHMS:  // TODO: Actuall implement that stuff
			extension.signatureAlgorithms = {};
			break;
		case TLSExtensionType::KEY_SHARE:
			extension.keyShares = parseTLSKeyShares(data);
			break;
		default:
			// TODO: Reenable this line of code / actually implement the extensions
			// Logger::debug("Received extension of type {:04X}, which currently isn't supported", static_cast<uint16_t>(type));
			break;
		}
	}

	return extension;
}
