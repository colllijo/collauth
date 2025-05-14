#pragma once

#include <cstdint>
#include <vector>

#include "math/Number.hpp"

std::vector<uint8_t> generateRandomBytes(size_t length);

Number generateRandomNumber(const Number& max);
