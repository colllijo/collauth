#pragma once

#include <cstdint>
#include <tuple>
#include <vector>

[[nodiscard]] std::tuple<std::vector<uint64_t>, std::vector<uint64_t>> divideDigits(const std::vector<uint64_t>& dividend, const std::vector<uint64_t>& divisor);

[[nodiscard]] bool getBit(const std::vector<uint64_t>& digits, size_t i);
void setBit(std::vector<uint64_t>& digits, size_t i, bool value);
