#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

enum class TLSServerNameType : uint8_t
{
	HOST_NAME = 0x00,
};

std::unordered_map<TLSServerNameType, std::string> parseTLSServerName(const std::vector<uint8_t>& data);
