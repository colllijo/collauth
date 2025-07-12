#pragma once

#include <compare>
#include <cstdint>
#include <vector>

[[nodiscard]] std::strong_ordering compareDigits(const std::vector<uint64_t>& lhs, const std::vector<uint64_t>& rhs) noexcept;
