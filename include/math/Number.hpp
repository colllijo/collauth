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
	Number();

	template <std::integral T>
	Number(T value)
	{
		fromIntegral(value);
	}

	explicit Number(std::string value);
	explicit Number(const std::vector<uint32_t>& digits, bool negative = false);

	constexpr static uint64_t MASK32 = (1ULL << 32) - 1;

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

	Number operator<<(size_t count) const;
	Number operator>>(size_t count) const;

	Number& operator<<=(size_t count);
	Number& operator>>=(size_t count);

	bool operator==(const Number& other) const;
	std::strong_ordering operator<=>(const Number& other) const;

	Number& pow(const Number& exponent);
	Number& modPow(const Number& exponent, const Number& modulus);

	Number gcd(const Number& other) const;
	Number modInverse(const Number& modulus) const;

	void rightShift(size_t count);
	void leftShift(size_t count);
	void rightShiftDigit(size_t count);
	void leftShiftDigit(size_t count);

	std::strong_ordering compareAbs(const Number& other) const;

	std::vector<uint32_t> getDigits() const;

	static Number pow(Number base, Number exponent);
	static Number modPow(Number base, Number exponent, const Number& modulus);

	static Number gcd(Number a, Number b);
	static Number modInverse(Number a, Number modulus);

	static Number fromString(const std::string& str);
	std::string toString() const;

	template <std::integral T>
	void fromIntegral(T number);

	friend std::ostream& operator<<(std::ostream& os, const Number& number);
	friend struct std::formatter<Number>;

private:
	std::vector<uint32_t> digits;
	bool negative;

	std::vector<uint32_t> add(const std::vector<uint32_t>& a, const std::vector<uint32_t>& b) const;
	std::vector<uint32_t> sub(const std::vector<uint32_t>& a, const std::vector<uint32_t>& b) const;
	std::vector<uint32_t> mul(const std::vector<uint32_t>& a, const std::vector<uint32_t>& b) const;
	std::tuple<std::vector<uint32_t>, std::vector<uint32_t>> div(const std::vector<uint32_t>& a, const std::vector<uint32_t>& b) const;

	std::vector<uint32_t> bitShiftRight(const std::vector<uint32_t>& digits, size_t count) const;
	std::vector<uint32_t> bitShiftLeft(const std::vector<uint32_t>& digits, size_t count) const;
	std::vector<uint32_t> digitShiftRight(const std::vector<uint32_t>& digits, size_t count) const;
	std::vector<uint32_t> digitShiftLeft(const std::vector<uint32_t>& digits, size_t count) const;

	static std::tuple<Number, Number, Number> extendedGCD(Number a, Number b);

	void trimLeadingZeros();
};

template <std::integral T>
void Number::fromIntegral(T number)
{
	digits.clear();

	if constexpr (std::is_signed_v<T>)
	{
		negative = number < 0;
		uint64_t value = static_cast<uint64_t>(static_cast<std::make_unsigned_t<T>>(negative ? -(number + 1) + 1 : number));

		do
		{
			digits.push_back(static_cast<uint32_t>(value & MASK32));
			value >>= 32;
		} while (value != 0);
	}
	else
	{
		negative = false;
		uint64_t value = static_cast<uint64_t>(number);

		do
		{
			digits.push_back(static_cast<uint32_t>(value & MASK32));
			value >>= 32;
		} while (value != 0);
	}

	trimLeadingZeros();
}

template <>
struct std::formatter<Number> : std::formatter<std::string>
{
	auto format(const Number& number, std::format_context& ctx) const { return std::formatter<std::string>::format(number.toString(), ctx); }
};
