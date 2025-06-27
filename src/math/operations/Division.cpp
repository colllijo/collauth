#include "math/operations/Division.hpp"

#include <compare>
#include <stdexcept>

#include "math/operations/Comparison.hpp"
#include "math/operations/Shifting.hpp"
#include "math/operations/Subtraction.hpp"

std::tuple<std::vector<uint32_t>, std::vector<uint32_t>> divideDigits(const std::vector<uint32_t>& dividend, const std::vector<uint32_t>& divisor)
{
	if (divisor.empty()) throw std::invalid_argument("Division by zero.");
	if (dividend.empty()) return {{}, {}};

	std::vector<uint32_t> quotient(dividend.size(), 0);
	std::vector<uint32_t> remainder(divisor.size(), 0);

	for (size_t i = dividend.size() * 32; i-- > 0;)
	{
		remainder = bitShiftLeft(remainder, 1);
		setBit(remainder, 0, getBit(dividend, i));

		if (compareDigits(remainder, divisor) != std::strong_ordering::less)
		{
			remainder = subtractDigits(remainder, divisor);
			setBit(quotient, i, 1);
		}
	}

	while (!quotient.empty() && quotient.back() == 0) quotient.pop_back();
	while (!remainder.empty() && remainder.back() == 0) remainder.pop_back();

	return {quotient, remainder};
}

bool getBit(const std::vector<uint32_t>& digits, size_t i)
{
	size_t index = i / 32;
	size_t bitPos = i % 32;
	if (index >= digits.size())
	{
		throw std::out_of_range("Bit index out of range");
	}
	return (digits[index] & (1U << bitPos)) != 0;
}

void setBit(std::vector<uint32_t>& digits, size_t i, bool value)
{
	size_t index = i / 32;
	size_t bitPos = i % 32;
	if (index >= digits.size())
	{
		throw std::out_of_range("Bit index out of range");
	}
	if (value)
	{
		digits[index] |= (1U << bitPos);  // Set the bit to 1
	}
	else
	{
		digits[index] &= ~(1U << bitPos);  // Clear the bit to 0
	}
}
