#pragma once

#include <compare>
#include <concepts>
#include <cstdint>
#include <format>
#include <string>
#include <tuple>
#include <vector>

class Number
{
public:
	/*******************************************
	 * Constructors
	 *******************************************/

	Number();

	template <std::integral T>
	Number(T value);

	explicit Number(const std::string& value, uint32_t base = 10);
	explicit Number(const std::vector<uint64_t>& digits, bool negative = false);

	constexpr static uint64_t BASE = 64;
	constexpr static uint64_t MASK = 0xFFFFFFFFFFFFFFFF;

	/*******************************************
	 * Basic arithmetic operations
	 *******************************************/

	Number operator+(const Number& other) const;
	Number operator-(const Number& other) const;
	Number operator*(const Number& other) const;
	Number operator/(const Number& other) const;
	Number operator%(const Number& other) const;

	Number& operator+=(const Number& other);
	Number& operator-=(const Number& other);
	Number& operator*=(const Number& other);
	Number& operator/=(const Number& other);
	Number& operator%=(const Number& other);

	Number operator-() const;

	/*******************************************
	 * Shifting operations
	 *******************************************/

	Number operator<<(size_t count) const;
	Number operator>>(size_t count) const;

	Number& operator<<=(size_t count);
	Number& operator>>=(size_t count);

	void rightShift(size_t count);
	void leftShift(size_t count);
	void rightShiftDigit(size_t count);
	void leftShiftDigit(size_t count);

	/*******************************************
	 * Comparison operations
	 *******************************************/

	bool operator==(const Number& other) const;
	std::strong_ordering operator<=>(const Number& other) const;

	bool isEven() const;
	bool isOdd() const;

	/*******************************************
	 * Bitwise operations
	 *******************************************/

	Number operator&(const Number& other) const;
	Number operator|(const Number& other) const;

	/*******************************************
	 * Advanced arithmetic operations
	 *******************************************/

	Number pow(const Number& exponent) const;
	Number modPow(const Number& exponent, const Number& modulus) const;

	Number gcd(const Number& other) const;
	std::tuple<Number, Number, Number> extendedGCD(const Number& other) const;
	Number modInverse(const Number& modulus) const;

	static Number pow(Number base, Number exponent);
	static Number modPow(Number base, const Number& exponent, const Number& modulus);

	static Number gcd(Number a, Number b);
	static std::tuple<Number, Number, Number> extendedGCD(Number a, Number b);
	static Number modInverse(const Number& a, const Number& modulus);

	/*******************************************
	 * Information functions
	 *******************************************/

	bool isNegative() const;
	Number abs() const;

	size_t bits() const;
	size_t digis() const;
	std::vector<uint64_t> getDigits() const;

	/*******************************************
	 * Conversion functions
	 *******************************************/

	static Number fromString(const std::string& str, uint32_t base = 10);
	std::string toString() const;
	std::string toBinary() const;
	std::string toHex() const;

	template <std::integral T>
	void fromIntegral(T number);

	template <std::integral T>
	explicit operator T() const;

	friend std::ostream& operator<<(std::ostream& os, const Number& number);
	friend struct std::formatter<Number>;

private:
	std::vector<uint64_t> digits;
	bool negative;

	/*******************************************
	 * Conversion functions
	 *******************************************/

	void fromBinary(const std::string& binaryString);
	void fromDecimal(const std::string& decimalString);
	void fromHex(const std::string& hexString);

	void trimLeadingZeros();
};

template <std::integral T>
Number::Number(T value)
{
	fromIntegral(value);
	trimLeadingZeros();
}

template <std::integral T>
void Number::fromIntegral(T number)
{
	uint64_t value = static_cast<uint64_t>(number);

	if (std::is_signed_v<T> && number < 0)
	{
		negative = true;
		value = 0 - value;
	}

	digits.clear();
	digits.emplace_back(value);
}

template <std::integral T>
Number::operator T() const
{
	if (digits.empty()) return 0;

	uint64_t value = digits.at(0);

	if (std::is_signed_v<T> && negative) return static_cast<T>(-static_cast<std::make_signed_t<uint64_t>>(value));
	else return static_cast<T>(value);
}

template <std::integral T>
Number operator+(const T& lhs, const Number& rhs)
{
	return static_cast<Number>(lhs) + rhs;
}

template <std::integral T>
Number operator-(const T& lhs, const Number& rhs)
{
	return static_cast<Number>(lhs) - rhs;
}

template <std::integral T>
Number operator*(const T& lhs, const Number& rhs)
{
	return static_cast<Number>(lhs) * rhs;
}

template <std::integral T>
Number operator/(const T& lhs, const Number& rhs)
{
	return static_cast<Number>(lhs) / rhs;
}

template <std::integral T>
Number operator%(const T& lhs, const Number& rhs)
{
	return static_cast<Number>(lhs) % rhs;
}

template <>
struct std::formatter<Number> : std::formatter<std::string>
{
	auto format(const Number& number, std::format_context& ctx) const { return std::formatter<std::string>::format(number.toString(), ctx); }
};
