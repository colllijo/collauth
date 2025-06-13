#include "math/operations/Addition.hpp"

#include <algorithm>

std::vector<uint32_t> addDigits(const std::vector<uint32_t>& augend, const std::vector<uint32_t>& addend)
{
	size_t maxDigits = std::max(augend.size(), addend.size()) + 1;

	std::vector<uint32_t> sum;
	sum.reserve(maxDigits);

	uint64_t carry = 0;
	for (size_t i = 0; i < maxDigits - 1; ++i)
	{
		if (i < augend.size()) carry += augend[i];
		if (i < addend.size()) carry += addend[i];

		sum.emplace_back(static_cast<uint32_t>(carry));
		carry >>= 32;
	}

	if (carry) sum.emplace_back(static_cast<uint32_t>(carry));

	return sum;
}
