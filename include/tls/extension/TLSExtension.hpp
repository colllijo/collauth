#pragma once

#include <cstdint>
#include <unordered_map>
#include <vector>

#include "tls/TLSVersion.hpp"
#include "tls/extension/TLSKeyShare.hpp"
#include "tls/extension/TLSServerName.hpp"

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

struct TLSExtension
{
	TLSServerName serverName;
	std::vector<TLSKeyShareType> supportedGroups;
	std::vector<uint16_t> signatureAlgorithms;	// TODO: Implmenet Signature algorithms
	std::vector<TLSVersion> supportedVersions;
	std::unordered_map<TLSKeyShareType, std::vector<uint8_t>> keyShares;

	std::vector<uint8_t> serialize() const;
};

TLSExtension parseTLSExtensions(const std::vector<uint8_t>& data);
