#include "math/operations/Shifting.hpp"

std::vector<uint32_t> bitShiftRight(const std::vector<uint32_t> &digits, size_t count)
{
	if (digits.empty() || count == 0) return digits;

	size_t shift = count / 32;
	size_t bitShift = count % 32;

	if (shift >= digits.size()) return {};

	std::vector<uint32_t> result(digits.begin() + shift, digits.end());

	uint32_t carry = 0;
	for (size_t i = result.size(); i-- > 0;)
	{
		uint64_t temp = (static_cast<uint64_t>(carry) << 32) | result.at(i);
		result[i] = static_cast<uint32_t>(temp >> bitShift);
		carry = digits.at(i + shift);
	}

	while (!result.empty() && result.back() == 0) result.pop_back();

	return result;
}

std::vector<uint32_t> bitShiftLeft(const std::vector<uint32_t> &digits, size_t count)
{
	size_t shift = count / 32;
	size_t bitShift = count % 32;

	std::vector<uint32_t> result = digitShiftLeft(digits, shift);

	uint32_t carry = 0;
	for (size_t i = 0; i < result.size(); ++i)
	{
		uint64_t temp = (static_cast<uint64_t>(result[i]) << bitShift) | carry;
		result[i] = static_cast<uint32_t>(temp);
		carry = temp >> 32;
	}

	if (carry)
	{
		result.push_back(carry);
	}

	return result;
}

std::vector<uint32_t> digitShiftRight(const std::vector<uint32_t> &digits, size_t count)
{
	if (count >= digits.size()) return {};

	std::vector<uint32_t> result(digits.begin() + count, digits.end());

	while (!result.empty() && result.back() == 0) result.pop_back();
	return result;
}

std::vector<uint32_t> digitShiftLeft(const std::vector<uint32_t> &digits, size_t count)
{
	std::vector<uint32_t> result(count, 0);
	result.insert(result.end(), digits.begin(), digits.end());

	return result;
}
