#include "math/Number.hpp"

#include <algorithm>
#include <bitset>
#include <compare>
#include <format>
#include <stdexcept>
#include <string>
#include <tuple>
#include <utility>

#include "math/operations/Addition.hpp"
#include "math/operations/Subtraction.hpp"

namespace
{
	// String dividing decimal string by base for parsing
	std::tuple<std::string, uint64_t> divideByBase(const std::string &value, uint64_t base);

	// Utility functions for division
	uint32_t estimateQuotientDigit(const std::vector<uint32_t> &remainder, const std::vector<uint32_t> &divisor);
	void correctQuotientEstimate(std::vector<uint32_t> &remainder, const std::vector<uint32_t> &divisor, uint32_t &q_digit);
};	// namespace

/*******************************************
 * Constructors
 *******************************************/

Number::Number() : digits(), negative(false) {}
Number::Number(const std::string &value, uint32_t base)
{
	switch (base)
	{
	case 2:
		fromBinary(value);
		break;
	case 10:
		fromDecimal(value);
		break;
	case 16:
		fromHex(value);
		break;
	default:
		throw std::invalid_argument("Base not supported.");
	}
}

Number::Number(const std::vector<uint32_t> &digits, bool negative) : digits(digits), negative(negative) {}

/*******************************************
 * Basic arithmetic operations
 *******************************************/

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
		digits = addDigits(digits, other.digits);
		return *this;
	}

	auto cmp = *this <=> other;
	if (cmp == std::strong_ordering::greater)
	{
		digits = subtractDigits(digits, other.digits);
	}
	else if (cmp == std::strong_ordering::less)
	{
		digits = subtractDigits(other.digits, digits);
		negative = other.negative;
	}
	else if (cmp == std::strong_ordering::equal)
	{
		digits.clear();
		negative = false;
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
		digits = addDigits(digits, other.digits);
		return *this;
	}

	auto cmp = compareAbs(other);
	if (cmp == std::strong_ordering::greater)
	{
		digits = subtractDigits(digits, other.digits);
	}
	else if (cmp == std::strong_ordering::less)
	{
		digits = subtractDigits(other.digits, digits);
		negative = !other.negative;
	}
	else if (cmp == std::strong_ordering::equal)
	{
		negative = false;
		digits.clear();
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

	trimLeadingZeros();

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
	digits = std::get<0>(div(digits, other.digits));

	trimLeadingZeros();

	return *this;
}

Number Number::operator%(const Number &other) const
{
	Number result = *this;
	result %= other;

	return result;
}

Number &Number::operator%=(const Number &other)
{
	if (other.digits.empty())
	{
		throw std::invalid_argument("Division by zero.");
	}

	if (digits.empty())
	{
		return *this;
	}

	digits = std::get<1>(div(digits, other.digits));

	if (negative)
	{
		*this += other;
		negative = false;
	}

	trimLeadingZeros();

	return *this;
}

Number Number::operator-() const
{
	Number result = *this;
	result.negative = !result.negative;

	return result;
}

/*******************************************
 * Shifting operations
 *******************************************/

Number Number::operator<<(size_t count) const
{
	Number result = *this;
	result <<= count;

	return result;
}

Number Number::operator>>(size_t count) const
{
	Number result = *this;
	result >>= count;

	return *this;
}

Number &Number::operator<<=(size_t count)
{
	if (count == 0)
	{
		return *this;
	}

	digits = bitShiftLeft(digits, count);

	return *this;
}

Number &Number::operator>>=(size_t count)
{
	if (count == 0)
	{
		return *this;
	}

	digits = bitShiftRight(digits, count);

	return *this;
}

void Number::rightShift(size_t count)
{
	bitShiftRight(digits, count);
}

void Number::leftShift(size_t count)
{
	bitShiftLeft(digits, count);
}

void Number::rightShiftDigit(size_t count)
{
	digitShiftRight(digits, count);
}

void Number::leftShiftDigit(size_t count)
{
	digitShiftLeft(digits, count);
}

/*******************************************
 * Comparison operations
 *******************************************/

bool Number::operator==(const Number &other) const
{
	if (negative != other.negative)
	{
		return false;
	}
	if (digits.size() != other.digits.size())
	{
		return false;
	}

	for (size_t i = 0; i < digits.size(); ++i)
	{
		if (digits[i] != other.digits[i])
		{
			return false;
		}
	}

	return true;
}

std::strong_ordering Number::operator<=>(const Number &other) const
{
	if (*this == other)
	{
		return std::strong_ordering::equal;
	}
	if (negative != other.negative)
	{
		return negative ? std::strong_ordering::less : std::strong_ordering::greater;
	}

	auto cmp = compareAbs(other);

	if (negative)
	{
		if (cmp == std::strong_ordering::less)
		{
			return std::strong_ordering::greater;
		}
		if (cmp == std::strong_ordering::greater)
		{
			return std::strong_ordering::less;
		}
	}

	return cmp;
}

std::strong_ordering Number::compareAbs(const Number &other) const
{
	if (*this == other)
	{
		return std::strong_ordering::equal;
	}

	if (digits.size() != other.digits.size())
	{
		if (digits.size() < other.digits.size()) return std::strong_ordering::less;
		else return std::strong_ordering::greater;
	}
	else
	{
		for (size_t i = digits.size(); i-- > 0;)
		{
			if (digits.at(i) != other.digits.at(i))
			{
				if (digits.at(i) < other.digits.at(i)) return std::strong_ordering::less;
				else return std::strong_ordering::greater;
			}
		}
	}

	return std::strong_ordering::equal;
}

/*******************************************
 * Advanced arithmetic operations
 *******************************************/

Number Number::pow(const Number &exponent) const
{
	Number result = pow(*this, exponent);
	return result;
}

Number Number::modPow(const Number &exponent, const Number &modulus) const
{
	Number result = modPow(*this, exponent, modulus);
	return result;
}

Number Number::gcd(const Number &other) const
{
	return gcd(*this, other);
}

Number Number::modInverse(const Number &modulus) const
{
	return modInverse(*this, modulus);
}

Number Number::pow(Number base, Number exponent)
{
	Number result = 1;

	while (exponent != 0)
	{
		if (exponent % 2 != 0)
		{
			result *= base;
		}

		base *= base;
		exponent >>= 1;
	}

	return result;
}

Number Number::modPow(Number base, const Number &exponent, const Number &modulus)
{
	if (modulus == 1) return 0;
	base %= modulus;

	Number result = 1;
	for (const auto &bit : exponent.toBinaryString())
	{
		result = (result * result) % modulus;

		if (bit == '1')
		{
			result = (result * base) % modulus;
		}
	}

	return result;
}

Number Number::gcd(Number a, Number b)
{
	if (a == 0) return b;
	else if (b == 0) return a;

	a.negative = false;
	b.negative = false;

	size_t shiftA = a.bitLength() - (a.toBinaryString().find_last_of('1') + 1);
	size_t shiftB = b.bitLength() - (b.toBinaryString().find_last_of('1') + 1);

	a >>= shiftA;

	do
	{
		b >>= b.bitLength() - (b.toBinaryString().find_last_of('1') + 1);
		if (a > b)
		{
			std::swap(a, b);
		}

		b -= a;
	} while (b != 0);

	return a << std::min(shiftA, shiftB);
}

Number Number::modInverse(const Number &a, const Number &modulus)
{
	auto [gcd, x, y] = extendedGCD(a, modulus);

	if (gcd != 1) throw std::invalid_argument("Modular inverse does not exist.");

	return (x % modulus + modulus) % modulus;
}

/*******************************************
 * Information functions
 *******************************************/

size_t Number::bitLength() const
{
	if (digits.empty())
	{
		return 0;
	}

	size_t length = digits.size() * BASE;

	if (digits.back() != 0)
	{
		length -= __builtin_clz(digits.back());
	}

	return length;
}

std::vector<uint32_t> Number::getDigits() const
{
	return digits;
}

/*******************************************
 * Conversion functions
 *******************************************/

std::string Number::toString() const
{
	if (digits.empty())
	{
		return "0";
	}

	std::string result;
	Number tmp = *this;

	do
	{
		uint64_t carry = 0;

		for (size_t i = tmp.digits.size(); i-- > 0;)
		{
			carry = (carry << BASE) + tmp.digits.at(i);
			tmp.digits.at(i) = static_cast<uint32_t>((carry / 10) & MASK);
			carry %= 10;
		}

		result += static_cast<char>(carry + '0');

		while (!tmp.digits.empty() && tmp.digits.back() == 0)
		{
			tmp.digits.pop_back();
		}
	} while (!tmp.digits.empty());

	if (negative)
	{
		result += '-';
	}
	std::reverse(result.begin(), result.end());

	return result;
}

std::string Number::toBinaryString() const
{
	if (digits.empty()) return "0";

	std::string result;

	for (auto it = digits.rbegin(); it != digits.rend(); ++it)
	{
		result += std::bitset<32>(*it).to_string();
	}

	auto firstOne = result.find('1');
	if (firstOne != std::string::npos)
	{
		return result.substr(firstOne);
	}
	return "0";
}

std::string Number::toHexString() const
{
	if (digits.empty()) return "0";

	std::string result;

	for (auto it = digits.rbegin(); it != digits.rend(); ++it)
	{
		result += std::format("{:08x}", *it);
	}

	return result;
}

Number Number::fromString(const std::string &str, uint32_t base)
{
	switch (base)
	{
	case 2:
		return Number(str, 2);
	case 10:
		return Number(str, 10);
	case 16:
		return Number(str, 16);
	default:
		throw std::invalid_argument("Base not supported.");
	}
}

/*******************************************
 * Basic arithmetic operations
 *******************************************/

std::vector<uint32_t> Number::mul(const std::vector<uint32_t> &a, const std::vector<uint32_t> &b) const
{
	std::vector<uint32_t> result(a.size() + b.size());

	for (size_t i = 0; i < a.size(); ++i)
	{
		uint64_t carry = 0;
		for (size_t j = 0; j < b.size(); ++j)
		{
			carry += static_cast<uint64_t>(a.at(i)) * b.at(j) + result.at(i + j);
			result[i + j] = static_cast<uint32_t>(carry & MASK);
			carry >>= BASE;
		}
		result[i + b.size()] += static_cast<uint32_t>(carry);
	}

	return result;
}

std::tuple<std::vector<uint32_t>, std::vector<uint32_t>> Number::div(const std::vector<uint32_t> &a, const std::vector<uint32_t> &b) const
{
	if (b.empty()) throw std::invalid_argument("Division by zero.");
	if (a.empty()) return {};

	std::vector<uint32_t> quotient(a.size(), 0);
	std::vector<uint32_t> remainder;
	remainder.reserve(a.size() + 1);

	for (size_t i = a.size(); i-- > 0;)
	{
		remainder.insert(remainder.begin(), a.at(i));

		while (!remainder.empty() && remainder.back() == 0) remainder.pop_back();

		uint32_t q_digit = estimateQuotientDigit(remainder, b);
		correctQuotientEstimate(remainder, b, q_digit);

		quotient[i] = q_digit;
	}

	while (!quotient.empty() && quotient.back() == 0) quotient.pop_back();
	while (!remainder.empty() && remainder.back() == 0) remainder.pop_back();

	return {quotient, remainder};
}

/*******************************************
 * Shifting operations
 *******************************************/

std::vector<uint32_t> Number::bitShiftRight(const std::vector<uint32_t> &digits, size_t count) const
{
	if (digits.empty() || count == 0) return digits;

	size_t shift = count / BASE;
	size_t bitShift = count % BASE;

	if (shift >= digits.size()) return {};

	std::vector<uint32_t> result(digits.begin() + shift, digits.end());

	uint32_t carry = 0;
	for (size_t i = result.size(); i-- > 0;)
	{
		uint64_t temp = (static_cast<uint64_t>(carry) << BASE) | result.at(i);
		result[i] = (temp >> bitShift) & MASK;
		carry = digits.at(i + shift);
	}

	while (!result.empty() && result.back() == 0) result.pop_back();

	return result;
}

std::vector<uint32_t> Number::bitShiftLeft(const std::vector<uint32_t> &digits, size_t count) const
{
	size_t shift = count / BASE;
	size_t bitShift = count % BASE;

	std::vector<uint32_t> result = digitShiftLeft(digits, shift);

	uint32_t carry = 0;
	for (size_t i = 0; i < result.size(); ++i)
	{
		uint64_t temp = (static_cast<uint64_t>(result[i]) << bitShift) | carry;
		result[i] = temp & MASK;
		carry = temp >> BASE;
	}

	if (carry)
	{
		result.push_back(carry);
	}

	return result;
}

std::vector<uint32_t> Number::digitShiftRight(const std::vector<uint32_t> &digits, size_t count) const
{
	if (count >= digits.size()) return {};

	std::vector<uint32_t> result(digits.begin() + count, digits.end());

	while (!result.empty() && result.back() == 0) result.pop_back();
	return result;
}

std::vector<uint32_t> Number::digitShiftLeft(const std::vector<uint32_t> &digits, size_t count) const
{
	std::vector<uint32_t> result(count, 0);
	result.insert(result.end(), digits.begin(), digits.end());

	return result;
}

/*******************************************
 * Advanced arithmetic operations
 *******************************************/

std::tuple<Number, Number, Number> Number::extendedGCD(Number a, Number b)
{
	Number x = 1, y = 0;
	Number x1 = 0, y1 = 1;

	while (b != 0)
	{
		Number q = a / b;

		std::tie(x, x1) = std::make_tuple(x1, x - q * x1);
		std::tie(y, y1) = std::make_tuple(y1, y - q * y1);
		std::tie(a, b) = std::make_tuple(b, a - q * b);
	}

	return std::make_tuple(a, x, y);
}

/*******************************************
 * Conversion functions
 *******************************************/

void Number::fromBinary(const std::string &binaryString)
{
	if (binaryString.empty())
	{
		throw std::invalid_argument("Value is required to create a number.");
	}

	// Initialize the digits vector with one element to avoid out-of-bounds access.
	digits.push_back(0);

	for (char bit : binaryString)
	{
		if (bit != '0' && bit != '1')
		{
			throw std::invalid_argument("Invalid character in binary string.");
		}

		digits = bitShiftLeft(digits, 1);
		if (bit == '1')
		{
			digits[0] |= 1;
		}
	}

	trimLeadingZeros();
}

void Number::fromDecimal(const std::string &decimalString)
{
	const uint64_t base = 1ULL << BASE;

	if (decimalString.empty())
	{
		throw std::invalid_argument("Value is required to create a number.");
	}

	std::string representation = decimalString;

	if (representation.at(0) == '-' || representation.at(0) == '+')
	{
		negative = representation.at(0) == '-';
		representation = representation.substr(1);

		// Check that there actually is a number not just a sign.
		if (representation.empty())
		{
			throw std::invalid_argument("Value is required to create a number.");
		}
	}

	do
	{
		auto [quotient, remainder] = divideByBase(representation, base);

		representation = quotient;
		digits.push_back(static_cast<uint32_t>(remainder & MASK));
	} while (!representation.empty());

	trimLeadingZeros();
}

void Number::fromHex(const std::string &hexString)
{
	if (hexString.empty())
	{
		throw std::invalid_argument("Value is required to create a number.");
	}

	// Initialize the digits vector with one element to avoid out-of-bounds access.
	digits.push_back(0);

	for (char digit : hexString)
	{
		if (digit >= '0' && digit <= '9')
		{
			digit -= '0';
		}
		else if (digit >= 'A' && digit <= 'F')
		{
			digit -= 'A' - 10;
		}
		else if (digit >= 'a' && digit <= 'f')
		{
			digit -= 'a' - 10;
		}
		else
		{
			throw std::invalid_argument("Invalid character in binary string.");
		}

		digits = bitShiftLeft(digits, 4);
		digits[0] |= digit;
	}

	trimLeadingZeros();
}

std::ostream &operator<<(std::ostream &os, const Number &number)
{
	os << number.toString();
	return os;
}

void Number::trimLeadingZeros()
{
	while (!digits.empty() && digits.back() == 0)
	{
		digits.pop_back();
	}
	if (digits.empty())
	{
		negative = false;
	}
}

namespace
{
	std::tuple<std::string, uint64_t> divideByBase(const std::string &value, uint64_t base)
	{
		std::string quotient = "";
		uint64_t remainder = 0;

		for (size_t i = 0; i < value.size(); ++i)
		{
			if (value.at(i) < '0' || value.at(i) > '9')
			{
				throw std::invalid_argument("Invalid character in number string.");
			}

			remainder = remainder * 10 + value.at(i) - '0';

			quotient += static_cast<char>((remainder / base) + '0');
			remainder %= base;
		}

		while (!quotient.empty() && quotient.at(0) == '0')
		{
			quotient = quotient.substr(1);
		}

		return {quotient, remainder};
	}

	uint32_t estimateQuotientDigit(const std::vector<uint32_t> &remainder, const std::vector<uint32_t> &divisor)
	{
		if (remainder.size() < divisor.size())
		{
			return 0;
		}

		size_t n = divisor.size();
		size_t m = remainder.size();

		uint64_t divisorHigh = divisor.at(n - 1);
		uint64_t remainderHigh = remainder.at(m - 1);

		if (m > n)
		{
			remainderHigh = remainderHigh << Number::BASE | remainder.at(m - 2);
		}

		auto q_digit = static_cast<uint32_t>((remainderHigh / divisorHigh) & Number::MASK);

		return q_digit;
	}

	void correctQuotientEstimate(std::vector<uint32_t> &remainder, const std::vector<uint32_t> &divisor, uint32_t &q_digit)
	{
		if (q_digit == 0)
		{
			return;
		}

		Number div = Number(divisor);
		Number rem = Number(remainder);

		Number product = div * q_digit;
		rem -= product;

		if (rem < 0)
		{
			q_digit--;
			rem += div;
		}

		remainder = rem.getDigits();
	}
};	// namespace
