#pragma once

#include <cstdint>
#include <tuple>
#include <vector>

enum class TLSContentType : uint8_t
{
	CHANGE_CIPHER_SPEC = 0x14,
	HANDSHAKE = 0x16,
	APPLICATION_DATA = 0x17,
};

enum class TLSHandshakeType : uint8_t
{
	CLIENT_HELLO = 0x01,
	SERVER_HELLO = 0x02
};

struct TLSHandshakeHeader
{
	TLSHandshakeType handshakeType;
	uint32_t length;
};

enum class TLSCipherSuite : uint16_t
{
	TLS_AES_128_GCM_SHA256 = 0x1301,
	TLS_AES_256_GCM_SHA384 = 0x1302,
	TLS_CHACHA20_POLY1305_SHA256 = 0x1303,
	TLS_AES_128_CCM_SHA256 = 0x1304,
	TLS_AES_128_CCM_8_SHA256 = 0x1305
};

std::tuple<TLSHandshakeType, std::vector<uint8_t>> parseTLSHandshake(const std::vector<uint8_t>& data);
std::vector<uint8_t> buildTLSHandshake(TLSHandshakeType handshakeType, const std::vector<uint8_t>& handshake);
