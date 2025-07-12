#pragma once

#include <cstdint>
#include <vector>

[[nodiscard]] std::vector<uint64_t> bitwiseAnd(const std::vector<uint64_t>& lhs, const std::vector<uint64_t>& rhs) noexcept;
[[nodiscard]] std::vector<uint64_t> bitwiseOr(const std::vector<uint64_t>& lhs, const std::vector<uint64_t>& rhs) noexcept;
