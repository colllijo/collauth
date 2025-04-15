#pragma once

#include <cstdint>
#include <vector>

enum class TLSVersion : uint16_t
{
	TLS_1_0 = 0x0301,
	TLS_1_1 = 0x0302,
	TLS_1_2 = 0x0303,
	TLS_1_3 = 0x0304
};

std::vector<TLSVersion> parseTLSVersion(const std::vector<uint8_t>& data);
