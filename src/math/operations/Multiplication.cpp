#include "math/operations/Multiplication.hpp"

#include <algorithm>

#include "math/operations/Addition.hpp"
#include "math/operations/Shifting.hpp"
#include "math/operations/Subtraction.hpp"

std::vector<uint64_t> multiplyDigits(const std::vector<uint64_t>& multiplier, const std::vector<uint64_t>& multiplicand)
{
	size_t bits = std::max(multiplier.size(), multiplicand.size()) << 5;
	std::vector<uint64_t> product;

	if (bits <= 64) product = hardwareMultiplication(multiplier, multiplicand);
	if (bits <= 256) product =  longMultiplication(multiplier, multiplicand);
	else product = karatsubaMultiplication(multiplier, multiplicand);  // Once a better algorithm is implemented limit karatsuba to 2048 digits.

	// Remove leading zeros
	while (!product.empty() && product.back() == 0) product.pop_back();

	return product;
}

std::vector<uint64_t> hardwareMultiplication(const std::vector<uint64_t>& multiplier, const std::vector<uint64_t>& multiplicand)
{
	if (multiplier.empty() || multiplicand.empty()) return {};

	__uint128_t product = 1;

	if (!multiplier.empty()) product *= multiplier[0];
	if (!multiplicand.empty()) product *= multiplicand[0];

	return {
		static_cast<uint64_t>(product),
		static_cast<uint64_t>(product >> 64),
	};
}

std::vector<uint64_t> longMultiplication(const std::vector<uint64_t>& multiplier, const std::vector<uint64_t>& multiplicand)
{
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

std::vector<uint64_t> karatsubaMultiplication(const std::vector<uint64_t>& multiplier, const std::vector<uint64_t>& multiplicand)
{
	if (multiplier.size() <= 1 && multiplicand.size() <= 1) return hardwareMultiplication(multiplier, multiplicand);

	size_t m = std::max(multiplier.size(), multiplicand.size()) >> 1;

	auto [lowMultiplier, highMultiplier] = splitVector(multiplier, m);
	auto [lowMultiplicand, highMultiplicand] = splitVector(multiplicand, m);

	std::vector<uint64_t> z0 = karatsubaMultiplication(lowMultiplier, lowMultiplicand);
	std::vector<uint64_t> z1 = karatsubaMultiplication(addDigits(lowMultiplier, highMultiplier), addDigits(lowMultiplicand, highMultiplicand));
	std::vector<uint64_t> z2 = karatsubaMultiplication(highMultiplier, highMultiplicand);

	return addDigits(addDigits((digitShiftLeft(z2, (m << 1))), (digitShiftLeft((subtractDigits(subtractDigits(z1, z2), z0)), m))), z0);
}

std::pair<std::vector<uint64_t>, std::vector<uint64_t>> splitVector(const std::vector<uint64_t>& vec, size_t index)
{
	index = std::min(index, vec.size());

	std::vector<uint64_t> lower(vec.begin(), vec.begin() + index);
	std::vector<uint64_t> upper(vec.begin() + index, vec.end());

	return {lower, upper};
}
