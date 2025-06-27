#pragma once

#include <cstdint>
#include <tuple>
#include <vector>

std::tuple<std::vector<uint32_t>, std::vector<uint32_t>> divideDigits(const std::vector<uint32_t>& dividend, const std::vector<uint32_t>& divisor);

bool getBit(const std::vector<uint32_t>& digits, size_t i);
void setBit(std::vector<uint32_t>& digits, size_t i, bool value);
