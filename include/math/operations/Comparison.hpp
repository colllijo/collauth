#pragma once

#include <compare>
#include <cstdint>
#include <vector>

std::strong_ordering compareDigits(const std::vector<uint32_t>& lhs, const std::vector<uint32_t>& rhs);
