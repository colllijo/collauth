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

	[[nodiscard]] Number operator+(const Number& other) const noexcept;
	[[nodiscard]] Number operator-(const Number& other) const noexcept;
	[[nodiscard]] Number operator*(const Number& other) const noexcept;
	[[nodiscard]] Number operator/(const Number& other) const;
	[[nodiscard]] Number operator%(const Number& other) const;

	Number& operator+=(const Number& other) noexcept;
	Number& operator-=(const Number& other) noexcept;
	Number& operator*=(const Number& other) noexcept;
	Number& operator/=(const Number& other);
	Number& operator%=(const Number& other);

	[[nodiscard]] Number operator-() const;

	/*******************************************
	 * Shifting operations
	 *******************************************/

	[[nodiscard]] Number operator<<(size_t count) const noexcept;
	[[nodiscard]] Number operator>>(size_t count) const noexcept;

	Number& operator<<=(size_t count) noexcept;
	Number& operator>>=(size_t count) noexcept;

	Number& rightShift(size_t count) noexcept;
	Number& leftShift(size_t count) noexcept;
	Number& rightShiftDigit(size_t count) noexcept;
	Number& leftShiftDigit(size_t count) noexcept;

	/*******************************************
	 * Comparison operations
	 *******************************************/

	[[nodiscard]] bool operator==(const Number& other) const noexcept;
	[[nodiscard]] std::strong_ordering operator<=>(const Number& other) const noexcept;

	[[nodiscard]] bool isEven() const noexcept;
	[[nodiscard]] bool isOdd() const noexcept;

	/*******************************************
	 * Bitwise operations
	 *******************************************/

	[[nodiscard]] Number operator&(const Number& other) const noexcept;
	[[nodiscard]] Number operator|(const Number& other) const noexcept;

	Number &operator&=(const Number& other) noexcept;
	Number &operator|=(const Number& other) noexcept;

	/*******************************************
	 * Advanced arithmetic operations
	 *******************************************/

	[[nodiscard]] Number pow(const Number& exponent) const noexcept;
	[[nodiscard]] Number modPow(const Number& exponent, const Number& modulus) const noexcept;

	[[nodiscard]] Number gcd(const Number& other) const noexcept;
	[[nodiscard]] std::tuple<Number, Number, Number> extendedGCD(const Number& other) const noexcept;
	[[nodiscard]] Number modInverse(const Number& modulus) const;

	[[nodiscard]] static Number pow(Number base, Number exponent) noexcept;
	[[nodiscard]] static Number modPow(Number base, const Number& exponent, const Number& modulus) noexcept;

	[[nodiscard]] static Number gcd(Number a, Number b) noexcept;
	[[nodiscard]] static std::tuple<Number, Number, Number> extendedGCD(Number a, Number b) noexcept;
	[[nodiscard]] static Number modInverse(const Number& a, const Number& modulus);

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
Number::Number(T value): digits(), negative(false)
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

	uint64_t value = digits[0];

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
