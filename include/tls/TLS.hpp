#pragma once

#include <cstdint>
#include <tuple>
#include <unordered_map>
#include <vector>

enum class TLSContentType : uint8_t
{
	HANDSHAKE = 0x16
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

enum class TLSExtensionType : uint16_t
{
	SERVER_NAME = 0x0000,
	MAX_FRAGMENT_LENGTH = 0x0001,
	STATUS_REQUEST = 0x0005,
	SUPPORTED_GROUPS = 0x000A,
	SIGNATURE_ALGORITHMS = 0x000D,
	USE_SRTP = 0x000E,
	HEARTBEAT = 0x000F,
	APPLICATION_LAYER_PROTOCOL_NEGOTIATION = 0x0010,
	SIGNED_CERTIFICATE_TIMESTAMP = 0x0012,
	CLIENT_CERTIFICATE_TYPE = 0x0013,
	SERVER_CERTIFICATE_TYPE = 0x0014,
	PADDING = 0x0015,
	PRE_SHARED_KEY = 0x0029,
	EARLY_DATA = 0x002A,
	SUPPORTED_VERSIONS = 0x002B,
	COOKIE = 0x002C,
	PSK_KEY_EXCHANGE_MODES = 0x002D,
	CERTIFICATE_AUTHORITIES = 0x002F,
	OID_FILTERS = 0x0030,
	POST_HANDSHAKE_AUTH = 0x0031,
	SIGNATURE_ALGORITHMS_CERT = 0x0032,
	KEY_SHARE = 0x0033
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

std::unordered_map<TLSExtensionType, std::vector<uint8_t>> parseTLSExtensions(const std::vector<uint8_t>& data);
