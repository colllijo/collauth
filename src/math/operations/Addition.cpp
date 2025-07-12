#include "math/operations/Addition.hpp"

#include <algorithm>

[[nodiscard]]
std::vector<uint64_t> addDigits(const std::vector<uint64_t>& augend, const std::vector<uint64_t>& addend) noexcept
{
	if (augend.empty()) return addend;
	if (addend.empty()) return augend;

	size_t maxDigits = std::max(augend.size(), addend.size()) + 1;

	std::vector<uint64_t> sum;
	sum.reserve(maxDigits);

	__uint128_t carry = 0;
	for (size_t i = 0; i < maxDigits - 1; ++i)
	{
		if (i < augend.size()) carry += augend[i];
		if (i < addend.size()) carry += addend[i];

		sum.emplace_back(static_cast<uint64_t>(carry));
		carry >>= 64;
	}

	if (carry) sum.emplace_back(static_cast<uint64_t>(carry));

	return sum;
}
