#pragma once

#include <compare>
#include <cstdint>
#include <format>
#include <string>
#include <vector>

class Number
{
public:
	Number();
	Number(std::string value);

	Number operator+(const Number& other) const;
	Number operator-(const Number& other) const;
	Number operator*(const Number& other) const;
	Number operator/(const Number& other) const;

	Number& operator+=(const Number& other);
	Number& operator-=(const Number& other);
	Number& operator*=(const Number& other);
	Number& operator/=(const Number& other);

	bool operator==(const Number& other) const;
	std::strong_ordering operator<=>(const Number& other) const;

	std::strong_ordering compareAbs(const Number& other) const;

	static Number fromString(const std::string& str);
	std::string toString() const;

	friend std::ostream& operator<<(std::ostream& os, const Number& number);
	friend struct std::formatter<Number>;

private:
	std::vector<uint32_t> digits;
	bool negative;

	constexpr static uint64_t MASK32 = (1ULL << 32) - 1;

	std::vector<uint32_t> add(const std::vector<uint32_t>& a, const std::vector<uint32_t>& b) const;
	std::vector<uint32_t> sub(const std::vector<uint32_t>& a, const std::vector<uint32_t>& b) const;
	std::vector<uint32_t> mul(const std::vector<uint32_t>& a, const std::vector<uint32_t>& b) const;
	std::vector<uint32_t> div(const std::vector<uint32_t>& a, const std::vector<uint32_t>& b) const;

	void trimLeadingZeros();
};

template <>
struct std::formatter<Number> : std::formatter<std::string>
{
	auto format(const Number& number, std::format_context& ctx) const { return std::formatter<std::string>::format(number.toString(), ctx); }
};
