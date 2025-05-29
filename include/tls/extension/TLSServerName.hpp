#pragma once

#include <cstdint>
#include <string>
#include <vector>

enum class TLSServerNameType : uint8_t
{
	HOST_NAME = 0x00,
};

struct TLSServerName
{
	TLSServerNameType nameType;
	std::string name;

	std::vector<uint8_t> serialize() const;
};

TLSServerName parseTLSServerName(const std::vector<uint8_t>& data);
