#pragma once

#include <cstdint>
#include <vector>

#include "tls/TLS.hpp"

struct TLSPlaintext
{
	TLSContentType type;
	uint16_t version;
	std::vector<uint8_t> fragment;

	std::vector<uint8_t> serialize() const;
};

TLSPlaintext parseTLSPlaintext(const std::vector<uint8_t>& data);
TLSPlaintext buildTLSPlaintext(TLSContentType type, const std::vector<uint8_t>& fragment);
