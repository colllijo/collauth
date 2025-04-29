#include "math/Number.hpp"

#include <algorithm>
#include <compare>
#include <stdexcept>
#include <string>
#include <tuple>

namespace
{
	std::tuple<std::string, uint64_t> divideByBase(const std::string &value, uint64_t base);
};

Number::Number() : digits(), negative(false) {}
Number::Number(std::string value)
{
	*this = fromString(value);
}

Number Number::operator+(const Number &other) const
{
	Number result = *this;
	result += other;

	return result;
}

Number &Number::operator+=(const Number &other)
{
	if (negative == other.negative)
	{
		digits = add(digits, other.digits);
		return *this;
	}

	auto cmp = *this <=> other;
	if (cmp == std::strong_ordering::equal)
	{
		negative = false;
		digits.clear();
	}
	else if (cmp == std::strong_ordering::less)
	{
		negative = other.negative;
		digits = sub(other.digits, digits);
	}
	else
	{
		digits = sub(digits, other.digits);
	}

	return *this;
}

Number Number::operator-(const Number &other) const
{
	Number result = *this;
	result -= other;

	return result;
}

Number &Number::operator-=(const Number &other)
{
	if (negative != other.negative)
	{
		digits = add(digits, other.digits);
		return *this;
	}

	auto cmp = *this <=> other;

	if (cmp == std::strong_ordering::equal)
	{
		negative = false;
		digits.clear();
	}
	else if (cmp == std::strong_ordering::less)
	{
		negative = !other.negative;
		digits = sub(other.digits, digits);
	}
	else
	{
		digits = sub(digits, other.digits);
	}

	return *this;
}

Number Number::operator*(const Number &other) const
{
	Number result = *this;
	result *= other;

	return result;
}

Number &Number::operator*=(const Number &other)
{
	if (digits.empty() || other.digits.empty())
	{
		digits.clear();
		negative = false;
		return *this;
	}

	negative = negative != other.negative;
	digits = mul(digits, other.digits);

	return *this;
}

Number Number::operator/(const Number &other) const
{
	Number result = *this;
	result /= other;

	return result;
}

Number &Number::operator/=(const Number &other)
{
	if (other.digits.empty())
	{
		throw std::invalid_argument("Division by zero.");
	}

	if (digits.empty())
	{
		return *this;
	}

	negative = negative != other.negative;
	digits = div(digits, other.digits);

	return *this;
}

bool Number::operator==(const Number &other) const
{
	if (negative != other.negative) return false;
	if (digits.size() != other.digits.size()) return false;

	for (size_t i = 0; i < digits.size(); ++i)
	{
		if (digits[i] != other.digits[i]) return false;
	}

	return true;
}

std::strong_ordering Number::operator<=>(const Number &other) const
{
	if (*this == other) return std::strong_ordering::equal;
	if (negative != other.negative) return negative ? std::strong_ordering::less : std::strong_ordering::greater;

	auto cmp = compareAbs(other);

	if (negative)
	{
		if (cmp == std::strong_ordering::less) return std::strong_ordering::greater;
		if (cmp == std::strong_ordering::greater) return std::strong_ordering::less;
	}

	return cmp;
}

std::strong_ordering Number::compareAbs(const Number &other) const
{
	if (*this == other) return std::strong_ordering::equal;

	int cmp = 0;
	if (digits.size() != other.digits.size())
	{
		cmp = digits.size() < other.digits.size() ? -1 : 1;
	}
	else
	{
		for (size_t i = digits.size(); i-- > 0;)
		{
			if (digits[i] != other.digits[i])
			{
				cmp = digits[i] < other.digits[i] ? -1 : 1;
				break;
			}
		}
	}

	switch (cmp)
	{
	case -1:
		return std::strong_ordering::less;
	case 1:
		return std::strong_ordering::greater;
	default:
		return std::strong_ordering::equal;
	}
}

std::string Number::toString() const
{
	if (digits.empty()) return "0";

	std::string result;
	Number tmp = *this;

	do
	{
		uint64_t carry = 0;

		for (size_t i = tmp.digits.size(); i-- > 0;)
		{
			carry = (carry << 32) + tmp.digits[i];
			tmp.digits[i] = static_cast<uint32_t>((carry / 10) & MASK32);
			carry %= 10;
		}

		result += static_cast<char>(carry + '0');

		while (!tmp.digits.empty() && tmp.digits.back() == 0) tmp.digits.pop_back();
	} while (!tmp.digits.empty());

	if (negative) result += '-';
	std::reverse(result.begin(), result.end());

	return result;
}

Number Number::fromString(const std::string &str)
{
	const uint64_t base = 1ULL << 32;

	if (str.empty()) throw std::invalid_argument("Value is required to create a number.");

	std::string representation = str;
	Number result;

	if (representation[0] == '-' || representation[0] == '+')
	{
		result.negative = representation[0] == '-';
		representation = representation.substr(1);

		// Check that there actually is a number not just a sign.
		if (representation.empty()) throw std::invalid_argument("Value is required to create a number.");
	}

	do
	{
		auto [quotient, remainder] = divideByBase(representation, base);

		representation = quotient;
		result.digits.push_back(static_cast<uint32_t>(remainder & MASK32));
	} while (!representation.empty());

	result.trimLeadingZeros();
	return result;
}

std::ostream &operator<<(std::ostream &os, const Number &number)
{
	os << number.toString();
	return os;
}

std::vector<uint32_t> Number::add(const std::vector<uint32_t> &a, const std::vector<uint32_t> &b) const
{
	std::vector<uint32_t> result(std::max(a.size(), b.size()) + 1);

	uint64_t carry = 0;
	for (size_t i = 0; i < result.size(); ++i)
	{
		if (i < a.size()) carry += a[i];
		if (i < b.size()) carry += b[i];

		result[i] = static_cast<uint32_t>(carry & MASK32);
		carry >>= 32;
	}

	if (carry != 0)
	{
		result.push_back(static_cast<uint32_t>(carry));
	}

	return result;
}

std::vector<uint32_t> Number::sub(const std::vector<uint32_t> &a, const std::vector<uint32_t> &b) const
{
	std::vector<uint32_t> result(a.size());

	uint64_t borrow = 0;
	for (size_t i = 0; i < a.size() || i < b.size(); ++i)
	{
		if (i < a.size()) borrow += a[i];
		if (i < b.size()) borrow -= b[i];

		result[i] = static_cast<uint32_t>(borrow & MASK32);
		borrow >>= 32;
	}

	return result;
}

std::vector<uint32_t> Number::mul(const std::vector<uint32_t> &a, const std::vector<uint32_t> &b) const
{
	std::vector<uint32_t> result(a.size() + b.size());

	for (size_t i = 0; i < a.size(); ++i)
	{
		uint64_t carry = 0;
		for (size_t j = 0; j < b.size(); ++j)
		{
			carry += static_cast<uint64_t>(a[i]) * b[j] + result[i + j];
			result[i + j] = static_cast<uint32_t>(carry & MASK32);
			carry >>= 32;
		}
		result[i + b.size()] += static_cast<uint32_t>(carry);
	}

	return result;
}

std::vector<uint32_t> Number::div(const std::vector<uint32_t> &a, const std::vector<uint32_t> &b) const {}

void Number::trimLeadingZeros()
{
	while (!digits.empty() && digits.back() == 0) digits.pop_back();
	if (digits.empty()) negative = false;
}

namespace
{
	std::tuple<std::string, uint64_t> divideByBase(const std::string &value, uint64_t base)
	{
		std::string quotient = "";
		uint64_t remainder = 0;

		for (size_t i = 0; i < value.size(); ++i)
		{
			if (value.at(i) < '0' || value.at(i) > '9') throw std::invalid_argument("Invalid character in number string.");

			remainder = remainder * 10 + value.at(i) - '0';

			quotient += static_cast<char>((remainder / base) + '0');
			remainder %= base;
		}

		while (!quotient.empty() && quotient.at(0) == '0') quotient = quotient.substr(1);

		return {quotient, remainder};
	}
};	// namespace
