#include "math/Number.hpp"

#include <sys/types.h>

#include <algorithm>
#include <bitset>
#include <compare>
#include <cstdlib>
#include <format>
#include <stdexcept>
#include <string>
#include <tuple>
#include <utility>

#include "math/operations/Addition.hpp"
#include "math/operations/Bitwise.hpp"
#include "math/operations/Comparison.hpp"
#include "math/operations/Division.hpp"
#include "math/operations/Multiplication.hpp"
#include "math/operations/Shifting.hpp"
#include "math/operations/Subtraction.hpp"

namespace
{
	// String dividing decimal string by base for parsing
	std::tuple<std::string, uint64_t> divideByBase(const std::string &value);
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

Number::Number(const std::vector<uint64_t> &digits, bool negative) : digits(digits), negative(negative) {}

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

	auto cmp = compareDigits(digits, other.digits);
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

	negative ^= other.negative;
	digits = multiplyDigits(digits, other.digits);

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
	digits = std::get<0>(divideDigits(digits, other.digits));

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

	digits = std::get<1>(divideDigits(digits, other.digits));

	if (negative)
	{
		*this += other;
		negative = false;
	}

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

	return result;
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

	auto cmp = compareDigits(digits, other.digits);

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

bool Number::isEven() const
{
	return digits.empty() || (digits[0] & 1) == 0;
}

bool Number::isOdd() const
{
	return !digits.empty() && (digits[0] & 1) != 0;
}

/*******************************************
 * Comparison operations
 *******************************************/

Number Number::operator&(const Number &other) const
{
	return Number(bitwiseAnd(digits, other.digits), negative && other.negative);
}

Number Number::operator|(const Number &other) const
{
	return Number(bitwiseOr(digits, other.digits), negative || other.negative);
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

std::tuple<Number, Number, Number> Number::extendedGCD(const Number &other) const
{
	return extendedGCD(*this, other);
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
	for (const auto &bit : exponent.toBinary())
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
	a.negative = false;
	b.negative = false;

	if (a == 0) return b;
	else if (b == 0) return a;

	size_t k = (a | b).bits() - ((a | b).toBinary().find_last_of('1') + 1);
	a >>= k;
	b >>= k;

	do
	{
		b >>= b.bits() - (b.toBinary().find_last_of('1') + 1);
		if (a > b)
		{
			std::swap(a, b);
		}

		b -= a;
	} while (b != 0);

	return a << k;
}

std::tuple<Number, Number, Number> Number::extendedGCD(Number a, Number b)
{
	a.negative = false;
	b.negative = false;

	if (a == 0) return {b, 0, 1};
	if (b == 0) return {a, 1, 0};

	size_t k = (a | b).bits() - ((a | b).toBinary().find_last_of('1') + 1);
	a >>= k;
	b >>= k;

	Number a0 = a, b0 = b;

	Number x0 = 1, y0 = 0;
	Number x1 = 0, y1 = 1;

	while (a.isEven())
	{
		if (!(x0.isEven() && y0.isEven()))
		{
			x0 -= b0;
			y0 += a0;
		}

		a >>= 1;
		x0 >>= 1;
		y0 >>= 1;
	}

	do
	{
		while (b.isEven())
		{
			if (!(x1.isEven() && y1.isEven()))
			{
				x1 -= b0;
				y1 += a0;
			}
			b >>= 1;
			x1 >>= 1;
			y1 >>= 1;
		}

		if (a > b)
		{
			std::swap(a, b);
			std::swap(x0, x1);
			std::swap(y0, y1);
		}

		b = b - a;
		x1 = x1 - x0;
		y1 = y1 - y0;
	} while (b != 0);

	return {a << k, x0, y0};
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

bool Number::isNegative() const
{
	return negative;
}

Number Number::abs() const
{
	Number result = *this;
	result.negative = false;

	return result;
}

size_t Number::bits() const
{
	if (digits.empty())
	{
		return 0;
	}

	size_t length = digits.size() * BASE;

	if (digits.back() != 0)
	{
		length -= std::countl_zero(digits.back());
	}

	return length;
}

size_t Number::digis() const
{
	return digits.size();
}

std::vector<uint64_t> Number::getDigits() const
{
	return digits;
}

/*******************************************
 * Conversion functions
 *******************************************/

std::string Number::toString() const
{
	if (digits.empty()) return "0";

	std::string result;
	Number tmp = *this;

	do
	{
		__uint128_t carry = 0;

		for (size_t i = tmp.digits.size(); i-- > 0;)
		{
			carry = (carry) + tmp.digits[i];
			tmp.digits[i] = static_cast<__uint128_t>((carry / 10) & MASK);
			carry %= 10;
		}

		result += static_cast<char>(carry + '0');

		while (!tmp.digits.empty() && tmp.digits.back() == 0) tmp.digits.pop_back();
	} while (!tmp.digits.empty());

	if (negative) result += '-';
	std::reverse(result.begin(), result.end());

	return result;
}

std::string Number::toBinary() const
{
	if (digits.empty()) return "0";

	std::string result;

	for (auto it = digits.rbegin(); it != digits.rend(); ++it)
	{
		result += std::bitset<64>(*it).to_string();
	}

	auto start = result.find_first_not_of('0');
	if (start != std::string::npos) return result.substr(start);

	return "0";
}

std::string Number::toHex() const
{
	if (digits.empty()) return "0";

	std::string result;
	for (auto it = digits.rbegin(); it != digits.rend(); ++it) result += std::format("{:016X}", *it);

	size_t start = result.find_first_not_of('0');
	if (start != std::string::npos) return result.substr(start);

	return "0";
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
 * Conversion functions
 *******************************************/

void Number::fromBinary(const std::string &binaryString)
{
	if (binaryString.empty()) throw std::invalid_argument("Value is required to create a number.");

	std::string representation = binaryString;

	if (representation[0] == '-' || representation[0] == '+')
	{
		negative = representation[0] == '-';
		representation = representation.substr(1);

		// Check that there actually is a number not just a sign.
		if (representation.empty()) throw std::invalid_argument("Value is required to create a number.");
	}

	if (std::any_of(representation.begin(), representation.end(), [](const char c) { return c != '0' && c != '1'; }))
		throw std::invalid_argument("Invalid character in binary string.");

	// Initialize the digits vector with one element to avoid out-of-bounds access.
	digits.emplace_back(0);

	// OPTIMIZE: Count repeated zeros and shift them at once, instead of shifting one by one.
	for (char bit : binaryString)
	{
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
	if (decimalString.empty()) throw std::invalid_argument("Value is required to create a number.");

	std::string representation = decimalString;

	if (representation[0] == '-' || representation[0] == '+')
	{
		negative = representation[0] == '-';
		representation = representation.substr(1);

		// Check that there actually is a number not just a sign.
		if (representation.empty()) throw std::invalid_argument("Value is required to create a number.");
	}

	// Check that the representation only contains valid decimal digits.
	if (std::any_of(representation.begin(), representation.end(), [](const char c) { return c < '0' || c > '9'; }))
		throw std::invalid_argument("Invalid character in decimal string.");

	do
	{
		auto [quotient, remainder] = divideByBase(representation);

		representation = quotient;
		digits.emplace_back(remainder);
	} while (!representation.empty());

	trimLeadingZeros();
}

void Number::fromHex(const std::string &hexString)
{
	if (hexString.empty()) throw std::invalid_argument("Value is required to create a number.");

	std::string representation = hexString;
	std::transform(representation.begin(), representation.end(), representation.begin(), ::toupper);

	if (representation[0] == '-' || representation[0] == '+')
	{
		negative = representation[0] == '-';
		representation = representation.substr(1);

		// Check that there actually is a number not just a sign.
		if (representation.empty()) throw std::invalid_argument("Value is required to create a number.");
	}

	if (std::any_of(representation.begin(), representation.end(), [](const char c) { return (c < '0' || c > '9') && (c < 'A' || c > 'F'); }))
		throw std::invalid_argument("Invalid character in hex string.");

	// Initialize the digits vector with one element to avoid out-of-bounds access.
	digits.emplace_back(0);

	for (char digit : representation)
	{
		if (digit >= '0' && digit <= '9') digit -= '0';
		else if (digit >= 'A' && digit <= 'F') digit -= 'A' - 10;

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
	std::tuple<std::string, uint64_t> divideByBase(const std::string &value)
	{
		const __uint128_t BASE = static_cast<__uint128_t>(1) << 64;

		std::string quotient = "";
		__uint128_t remainder = 0;

		for (const auto &digit : value)
		{
			remainder = (remainder * 10) + (digit - '0');

			quotient += static_cast<char>((remainder / BASE) + '0');
			remainder %= BASE;
		}

		size_t leadingZeroes = quotient.find_first_not_of('0');
		quotient = quotient.substr(leadingZeroes == std::string::npos ? quotient.size() : leadingZeroes);

		return {quotient, remainder};
	}
};	// namespace
