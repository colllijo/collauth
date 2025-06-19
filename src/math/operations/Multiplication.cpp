#include "math/operations/Multiplication.hpp"

#include <algorithm>

#include "math/operations/Addition.hpp"
#include "math/operations/Subtraction.hpp"

std::vector<uint32_t> multiplyDigits(const std::vector<uint32_t>& multiplier, const std::vector<uint32_t>& multiplicand)
{
	size_t bits = std::max(multiplier.size(), multiplicand.size()) << 5;

	if (bits <= 32) return hardwareMultiplication(multiplier, multiplicand);
	if (bits <= 256) return longMultiplication(multiplier, multiplicand);
	return karatsubaMultiplication(multiplier, multiplicand);  // Once a better algorithm is implemented limit karatsuba to 2048 digits.
}

std::vector<uint32_t> hardwareMultiplication(const std::vector<uint32_t>& multiplier, const std::vector<uint32_t>& multiplicand)
{
	uint64_t product = 1;

	if (!multiplier.empty()) product *= multiplier[0];
	if (!multiplicand.empty()) product *= multiplicand[0];

	return {
		static_cast<uint32_t>(product),
		static_cast<uint32_t>(product >> 32),
	};
}

std::vector<uint32_t> longMultiplication(const std::vector<uint32_t>& multiplier, const std::vector<uint32_t>& multiplicand)
{
	size_t maxDigits = multiplier.size() + multiplicand.size();

	// Fill product with zeros, so that it can be index (unlike with reserve)
	std::vector<uint32_t> product(maxDigits, 0);

	for (size_t i = 0; i < multiplier.size(); ++i)
	{
		uint64_t carry = 0;
		for (size_t j = 0; j < multiplicand.size(); ++j)
		{
			uint64_t prod = static_cast<uint64_t>(multiplier[i]) * static_cast<uint64_t>(multiplicand[j]) + product[i + j] + carry;
			product[i + j] = static_cast<uint32_t>(prod);
			carry = prod >> 32;
		}

		product[i + multiplicand.size()] += static_cast<uint32_t>(carry);
	}

	// Remove leading zeros
	while (!product.empty() && product.back() == 0) product.pop_back();

	return product;
}

std::vector<uint32_t> karatsubaMultiplication(const std::vector<uint32_t>& multiplier, const std::vector<uint32_t>& multiplicand)
{
	// TODO: Add base condition
	if (multiplier.size() == 1 && multiplicand.size() == 1) return hardwareMultiplication(multiplier, multiplicand);

	size_t m = std::max(multiplier.size(), multiplicand.size()) >> 1;

	// TODO: Split vector in two
	auto [lowMultiplier, highMultiplier] = splitVector(multiplier, m);
	auto [lowMultiplicand, highMultiplicand] = splitVector(multiplicand, m);

	std::vector<uint32_t> z0 = karatsubaMultiplication(lowMultiplier, lowMultiplicand);
	std::vector<uint32_t> z1 = karatsubaMultiplication(addDigits(lowMultiplier, highMultiplier), addDigits(lowMultiplicand, highMultiplicand));
	std::vector<uint32_t> z2 = karatsubaMultiplication(highMultiplier, highMultiplicand);

	return addDigits(addDigits((leftShiftDigits(z2, (m << 1))), (leftShiftDigits((subtractDigits(subtractDigits(z1, z2), z0)), m))), z0);
}

std::pair<std::vector<uint32_t>, std::vector<uint32_t>> splitVector(const std::vector<uint32_t>& vec, size_t index)
{
	if (index > vec.size()) index = vec.size();

	std::vector<uint32_t> lower(vec.begin(), vec.begin() + index);
	std::vector<uint32_t> upper(vec.begin() + index, vec.end());

	return {lower, upper};
}

std::vector<uint32_t> leftShiftDigits(const std::vector<uint32_t>& input, size_t shift)
{
	if (input.empty() || shift == 0)
	{
		return input;
	}

	// Create a new vector with `shift` zeros appended at the end
	std::vector<uint32_t> result(input.size() + shift, 0);
	std::copy(input.begin(), input.end(), result.begin() + shift);

	return result;
}
