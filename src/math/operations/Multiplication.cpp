#include "math/operations/Multiplication.hpp"

#include <algorithm>
#include <bit>
#include <cassert>

#include "math/operations/Addition.hpp"
#include "math/operations/Shifting.hpp"
#include "math/operations/Subtraction.hpp"

[[nodiscard]]
std::vector<uint64_t> multiplyDigits(const std::vector<uint64_t>& multiplier, const std::vector<uint64_t>& multiplicand)
{
	size_t bits = std::max(multiplier.size(), multiplicand.size()) << 6;
	std::vector<uint64_t> product;

	if (bits <= 64) product = hardwareMultiplication(multiplier, multiplicand);
	else if (bits <= 256) product = longMultiplication(multiplier, multiplicand);
	else product = karatsubaMultiplication(multiplier, multiplicand);  // Once a better algorithm is implemented limit karatsuba to 2048 digits.

	// Remove leading zeros
	while (!product.empty() && product.back() == 0) product.pop_back();

	return product;
}

[[nodiscard]]
std::vector<uint64_t> hardwareMultiplication(const std::vector<uint64_t>& multiplier, const std::vector<uint64_t>& multiplicand)
{
	assert(multiplier.size() <= 1 && "Multiplier should be at most 1 digit");
	assert(multiplicand.size() <= 1 && "Multiplicand should be at most 1 digit");

	if (multiplier.empty() || multiplicand.empty()) return {};

	__uint128_t product = static_cast<__uint128_t>(multiplier[0]) * static_cast<__uint128_t>(multiplicand[0]);

	return {
		static_cast<uint64_t>(product),
		static_cast<uint64_t>(product >> 64),
	};
}

[[nodiscard]]
std::vector<uint64_t> longMultiplication(const std::vector<uint64_t>& multiplier, const std::vector<uint64_t>& multiplicand)
{
	if (multiplier.empty() || multiplicand.empty()) return {};

	size_t maxDigits = multiplier.size() + multiplicand.size();

	// Fill product with zeros, so that it can be index (unlike with reserve)
	std::vector<uint64_t> product(maxDigits, 0);

	for (size_t i = 0; i < multiplier.size(); ++i)
	{
		__uint128_t carry = 0;
		for (size_t j = 0; j < multiplicand.size(); ++j)
		{
			__uint128_t prod = static_cast<__uint128_t>(multiplier[i]) * static_cast<__uint128_t>(multiplicand[j]) + product[i + j] + carry;
			product[i + j] = static_cast<uint64_t>(prod);
			carry = prod >> 64;
		}

		product[i + multiplicand.size()] += static_cast<uint64_t>(carry);
	}

	return product;
}

[[nodiscard]]
std::vector<uint64_t> karatsubaMultiplication(std::vector<uint64_t> multiplier, std::vector<uint64_t> multiplicand)
{
	if (multiplier.size() <= 1 && multiplicand.size() <= 1) return hardwareMultiplication(multiplier, multiplicand);

	// Pad vectors to multiple of 2
	size_t maxSize = std::max(multiplier.size(), multiplicand.size());
	size_t padding = std::bit_ceil(maxSize);
	size_t m = padding >> 1;

	multiplier.resize(padding, 0);
	multiplicand.resize(padding, 0);

	auto [highMultiplier, lowMultiplier] = splitVector(multiplier, m);
	auto [highMultiplicand, lowMultiplicand] = splitVector(multiplicand, m);

	auto z0 = karatsubaMultiplication(lowMultiplier, lowMultiplicand);
	auto z2 = karatsubaMultiplication(highMultiplier, highMultiplicand);
	auto s = karatsubaMultiplication(addDigits(lowMultiplier, highMultiplier), addDigits(lowMultiplicand, highMultiplicand));

	auto z1 = subtractDigits(s, addDigits(z0, z2));

	// z2 * B^{2m} + (z1 − z2 − z0) * B^{m} + z0
	return (addDigits(digitShiftLeft(z2, m << 1), addDigits(digitShiftLeft(z1, m), z0)));
}

[[nodiscard]]
std::pair<std::vector<uint64_t>, std::vector<uint64_t>> splitVector(const std::vector<uint64_t>& vec, size_t index)
{
	assert(index <= vec.size() && index > 0 && "Index out of bounds for vector split");
	assert(vec.size() > 0 && (vec.size() & 0b1) == 0 && "Vector should be power of two in size");

	return {std::vector<uint64_t>(vec.begin() + index, vec.end()), std::vector<uint64_t>(vec.begin(), vec.begin() + index)};
}
