#pragma once

#include <cstdint>
#include <vector>

[[nodiscard]] std::vector<uint64_t> bitShiftRight(const std::vector<uint64_t>& digits, size_t count) noexcept;
[[nodiscard]] std::vector<uint64_t> bitShiftLeft(const std::vector<uint64_t>& digits, size_t count) noexcept;
[[nodiscard]] std::vector<uint64_t> digitShiftRight(const std::vector<uint64_t>& digits, size_t count) noexcept;
[[nodiscard]] std::vector<uint64_t> digitShiftLeft(const std::vector<uint64_t>& digits, size_t count) noexcept;
