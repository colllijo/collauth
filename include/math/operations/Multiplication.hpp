#pragma once

#include <cstdint>
#include <utility>
#include <vector>

std::vector<uint64_t> multiplyDigits(const std::vector<uint64_t>& multiplier, const std::vector<uint64_t>& multiplicand);

std::vector<uint64_t> hardwareMultiplication(const std::vector<uint64_t>& multiplier, const std::vector<uint64_t>& multiplicand);
std::vector<uint64_t> longMultiplication(const std::vector<uint64_t>& multiplier, const std::vector<uint64_t>& multiplicand);
std::vector<uint64_t> karatsubaMultiplication(const std::vector<uint64_t>& multiplier, const std::vector<uint64_t>& multiplicand);

std::pair<std::vector<uint64_t>, std::vector<uint64_t>> splitVector(const std::vector<uint64_t>& vec, size_t index);

std::vector<uint64_t> leftShiftDigits(const std::vector<uint64_t>& input, size_t shift);
