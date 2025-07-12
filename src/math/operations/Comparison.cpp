#include "math/operations/Comparison.hpp"

#include <algorithm>

[[nodiscard]]
std::strong_ordering compareDigits(const std::vector<uint64_t>& lhs, const std::vector<uint64_t>& rhs) noexcept
{
	if (lhs == rhs) return std::strong_ordering::equal;

	// Ensure that digits don't contain any leading zeros
	auto left = std::vector<uint64_t>(lhs.begin(), std::find_if_not(lhs.rbegin(), lhs.rend(), [](uint64_t digit) { return digit == 0; }).base());
	auto right = std::vector<uint64_t>(rhs.begin(), std::find_if_not(rhs.rbegin(), rhs.rend(), [](uint64_t digit) { return digit == 0; }).base());

	if (left.size() > right.size()) return std::strong_ordering::greater;
	else if (left.size() < right.size()) return std::strong_ordering::less;

	for (size_t i = left.size(); i-- > 0;)
	{
		if (left[i] > right[i]) return std::strong_ordering::greater;
		else if (left[i] < right[i]) return std::strong_ordering::less;
	}

	return std::strong_ordering::equal;
}
