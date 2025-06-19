#pragma once

#include <cstdint>
#include <utility>
#include <vector>

std::vector<uint32_t> multiplyDigits(const std::vector<uint32_t>& multiplier, const std::vector<uint32_t>& multiplicand);

std::vector<uint32_t> hardwareMultiplication(const std::vector<uint32_t>& multiplier, const std::vector<uint32_t>& multiplicand);
std::vector<uint32_t> longMultiplication(const std::vector<uint32_t>& multiplier, const std::vector<uint32_t>& multiplicand);
std::vector<uint32_t> karatsubaMultiplication(const std::vector<uint32_t>& multiplier, const std::vector<uint32_t>& multiplicand);

std::pair<std::vector<uint32_t>, std::vector<uint32_t>> splitVector(const std::vector<uint32_t>& vec, size_t index);

std::vector<uint32_t> leftShiftDigits(const std::vector<uint32_t>& input, size_t shift);
