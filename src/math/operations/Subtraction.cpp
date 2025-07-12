#include "math/operations/Subtraction.hpp"

#include <compare>
#include <stdexcept>

#include "math/operations/Comparison.hpp"

[[nodiscard]]
std::vector<uint64_t> subtractDigits(const std::vector<uint64_t>& minuend, const std::vector<uint64_t>& subtrahend)
{
	if (compareDigits(minuend, subtrahend) == std::strong_ordering::less) throw std::invalid_argument("Minuend must be greater than or equal to subtrahend for subtraction.");

	std::vector<uint64_t> difference;
	difference.reserve(minuend.size());

	__uint128_t borrow = 0;
	for (size_t i = 0; i < minuend.size(); ++i)
	{
		__uint128_t diff = minuend[i] - borrow;
		if (i < subtrahend.size()) diff -= subtrahend[i];

		difference.emplace_back(static_cast<uint64_t>(diff));
		borrow = (diff >> 64) > 0;
	}

	return difference;
}
