#include "math/operations/Bitwise.hpp"

std::vector<uint32_t> bitwiseAnd(const std::vector<uint32_t>& lhs, const std::vector<uint32_t>& rhs)
{
	size_t minSize = std::min(lhs.size(), rhs.size());

	std::vector<uint32_t> result;
	result.reserve(minSize);

	for (size_t i = 0; i < minSize; ++i) result.emplace_back(lhs[i] & rhs[i]);

	return result;
}

std::vector<uint32_t> bitwiseOr(const std::vector<uint32_t>& lhs, const std::vector<uint32_t>& rhs)
{
	size_t maxSize = std::max(lhs.size(), rhs.size());

	std::vector<uint32_t> result;
	result.reserve(maxSize);

	for (size_t i = 0; i < maxSize; ++i)
	{
		uint32_t value = 0;
		if (i < lhs.size()) value |= lhs[i];
		if (i < rhs.size()) value |= rhs[i];

		result.emplace_back(value);
	}

	return result;
}
