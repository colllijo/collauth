#include "math/operations/Comparison.hpp"

std::strong_ordering compareDigits(const std::vector<uint32_t>& lhs, const std::vector<uint32_t>& rhs)
{
	if (lhs == rhs) return std::strong_ordering::equal;

	if (lhs.size() > rhs.size()) return std::strong_ordering::greater;
	else if (lhs.size() < rhs.size()) return std::strong_ordering::less;

	for (size_t i = lhs.size(); i-- > 0;)
	{
		if (lhs[i] > rhs[i]) return std::strong_ordering::greater;
		else if (lhs[i] < rhs[i]) return std::strong_ordering::less;
	}

	return std::strong_ordering::equal;
}
