#include "tls/TLSSignature.hpp"

#include "networking/BufferReader.hpp"

std::vector<TLSSignatureScheme> parseTLSSignatureSchemes(const std::vector<uint8_t>& data)
{
	BufferReader reader(data);
	std::vector<TLSSignatureScheme> schemes;

	// Length of the signature schemes list
	uint16_t length = reader.readUint16();

	schemes.reserve(length / 2);
	for (size_t i = 0; i < length / 2; ++i)
	{
		schemes.push_back(static_cast<TLSSignatureScheme>(reader.readUint16()));
	}

	return schemes;
}
