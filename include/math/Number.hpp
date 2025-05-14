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
	constexpr Number();

	template <std::integral T>
	Number(T value)
	{
		fromIntegral(value);
	}

	explicit Number(const std::string& value, uint32_t base = 10);
	explicit Number(const std::vector<uint32_t>& digits, bool negative = false);

	constexpr static uint64_t MASK32 = (1ULL << 32ULL) - 1;

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

	Number operator<<(size_t count) const;
	Number operator>>(size_t count) const;

	Number& operator<<=(size_t count);
	Number& operator>>=(size_t count);

	bool operator==(const Number& other) const;
	std::strong_ordering operator<=>(const Number& other) const;

	Number pow(const Number& exponent) const;
	Number modPow(const Number& exponent, const Number& modulus) const;

	Number gcd(const Number& other) const;
	Number modInverse(const Number& modulus) const;

	void rightShift(size_t count);
	void leftShift(size_t count);
	void rightShiftDigit(size_t count);
	void leftShiftDigit(size_t count);

	size_t bitLength() const;

	std::strong_ordering compareAbs(const Number& other) const;

	std::vector<uint32_t> getDigits() const;

	static Number pow(Number base, Number exponent);
	static Number modPow(Number base, Number exponent, const Number& modulus);

	static Number gcd(Number a, Number b);
	static Number modInverse(Number a, const Number& modulus);

	static Number fromString(const std::string& str, uint32_t base = 10);
	std::string toString() const;
	std::string toBinaryString() const;

	template <std::integral T>
	void fromIntegral(T number);

	template <std::integral T>
	explicit operator T() const;

	friend std::ostream& operator<<(std::ostream& os, const Number& number);
	friend struct std::formatter<Number>;

private:
	std::vector<uint32_t> digits;
	bool negative{};

	std::vector<uint32_t> add(const std::vector<uint32_t>& a, const std::vector<uint32_t>& b) const;
	std::vector<uint32_t> sub(const std::vector<uint32_t>& a, const std::vector<uint32_t>& b) const;
	std::vector<uint32_t> mul(const std::vector<uint32_t>& a, const std::vector<uint32_t>& b) const;
	std::tuple<std::vector<uint32_t>, std::vector<uint32_t>> div(const std::vector<uint32_t>& a, const std::vector<uint32_t>& b) const;

	std::vector<uint32_t> bitShiftRight(const std::vector<uint32_t>& digits, size_t count) const;
	std::vector<uint32_t> bitShiftLeft(const std::vector<uint32_t>& digits, size_t count) const;
	std::vector<uint32_t> digitShiftRight(const std::vector<uint32_t>& digits, size_t count) const;
	std::vector<uint32_t> digitShiftLeft(const std::vector<uint32_t>& digits, size_t count) const;

	static std::tuple<Number, Number, Number> extendedGCD(Number a, Number b);

	void fromBinary(const std::string& binaryString);
	void fromDecimal(const std::string& decimalString);
	void fromHex(const std::string& hexString);

	void trimLeadingZeros();
};

template <std::integral T>
void Number::fromIntegral(T number)
{
	digits.clear();

	if constexpr (std::is_signed_v<T>)
	{
		negative = number < 0;
		auto value = static_cast<uint64_t>(static_cast<std::make_unsigned_t<T>>(negative ? -(number + 1) + 1 : number));

		do
		{
			digits.push_back(static_cast<uint32_t>(value & MASK32));
			value >>= 32;
		} while (value != 0);
	}
	else
	{
		negative = false;
		auto value = static_cast<uint64_t>(number);

		do
		{
			digits.push_back(static_cast<uint32_t>(value & MASK32));
			value >>= 32;
		} while (value != 0);
	}

	trimLeadingZeros();
}

template <std::integral T>
Number::operator T() const
{
	if (digits.empty())
	{
		return 0;
	}

	uint64_t value = digits.at(0);
	if (digits.size() > 1)
	{
		value = value << 32 | digits.at(1);
	}

	if (std::is_signed_v<T> && negative)
	{
		return static_cast<T>(-static_cast<std::make_signed_t<uint64_t>>(value));
	}
	else
	{
		return static_cast<T>(value);
	}
}

template <>
struct std::formatter<Number> : std::formatter<std::string>
{
	auto format(const Number& number, std::format_context& ctx) const { return std::formatter<std::string>::format(number.toString(), ctx); }
};
