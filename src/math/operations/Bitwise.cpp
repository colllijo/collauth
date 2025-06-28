#include "math/operations/Bitwise.hpp"

std::vector<uint32_t> bitwiseAnd(const std::vector<uint32_t>& lhs, const std::vector<uint32_t>& rhs)
{
	size_t minSize = std::min(lhs.size(), rhs.size());

	std::vector<uint32_t> result;
	result.reserve(minSize);

	for (size_t i = 0; i < minSize; ++i) result.emplace_back(lhs[i] & rhs[i]);

	return result;
}
