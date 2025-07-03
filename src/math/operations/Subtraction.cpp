#include "math/operations/Subtraction.hpp"

std::vector<uint64_t> subtractDigits(const std::vector<uint64_t>& minuend, const std::vector<uint64_t>& subtrahend)
{
	std::vector<uint64_t> difference;
	difference.reserve(minuend.size());

	uint64_t borrow = 0;
	for (size_t i = 0; i < minuend.size(); ++i)
	{
		uint64_t diff = minuend[i] - borrow;
		if (i < subtrahend.size()) diff -= subtrahend[i];

		difference.emplace_back(static_cast<uint64_t>(diff));
		borrow = (diff >> 32) > 0;
	}

	return difference;
}
