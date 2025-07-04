#pragma once

#include <cstdint>
#include <utility>
#include <vector>

[[nodiscard]] std::vector<uint64_t> multiplyDigits(const std::vector<uint64_t>& multiplier, const std::vector<uint64_t>& multiplicand);

[[nodiscard]] std::vector<uint64_t> hardwareMultiplication(const std::vector<uint64_t>& multiplier, const std::vector<uint64_t>& multiplicand);
[[nodiscard]] std::vector<uint64_t> longMultiplication(const std::vector<uint64_t>& multiplier, const std::vector<uint64_t>& multiplicand);
[[nodiscard]] std::vector<uint64_t> karatsubaMultiplication(std::vector<uint64_t> multiplier, std::vector<uint64_t> multiplicand);

[[nodiscard]] std::pair<std::vector<uint64_t>, std::vector<uint64_t>> splitVector(const std::vector<uint64_t>& vec, size_t index);
