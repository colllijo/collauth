#include "math/operations/Shifting.hpp"

[[nodiscard]]
std::vector<uint64_t> bitShiftRight(const std::vector<uint64_t> &digits, size_t count)
{
	if (digits.empty() || count == 0) return digits;

	size_t shift = count / 64;
	size_t bitShift = count % 64;

	if (shift >= digits.size()) return {};

	std::vector<uint64_t> result(digits.begin() + shift, digits.end());

	__uint128_t carry = 0;
	for (size_t i = result.size(); i-- > 0;)
	{
		__uint128_t temp = (static_cast<__uint128_t>(carry) << 64) | result[i];
		result[i] = static_cast<uint64_t>(temp >> bitShift);
		carry = digits[i + shift];
	}

	while (!result.empty() && result.back() == 0) result.pop_back();

	return result;
}

[[nodiscard]]
std::vector<uint64_t> bitShiftLeft(const std::vector<uint64_t> &digits, size_t count)
{
	size_t shift = count / 64;
	size_t bitShift = count % 64;

	std::vector<uint64_t> result = digitShiftLeft(digits, shift);

	__uint128_t carry = 0;
	for (size_t i = 0; i < result.size(); ++i)
	{
		__uint128_t temp = (static_cast<__uint128_t>(result[i]) << bitShift) | carry;
		result[i] = static_cast<uint64_t>(temp);
		carry = temp >> 64;
	}

	if (carry) result.emplace_back(carry);

	return result;
}

[[nodiscard]]
std::vector<uint64_t> digitShiftRight(const std::vector<uint64_t> &digits, size_t count)
{
	if (count >= digits.size()) return {};

	std::vector<uint64_t> result(digits.begin() + count, digits.end());

	while (!result.empty() && result.back() == 0) result.pop_back();
	return result;
}

[[nodiscard]]
std::vector<uint64_t> digitShiftLeft(const std::vector<uint64_t> &digits, size_t count)
{
	std::vector<uint64_t> result(count, 0);
	result.insert(result.end(), digits.begin(), digits.end());

	return result;
}
