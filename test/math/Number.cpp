#include "math/Number.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE(
	"Number constructor from integral type"
	"[math][number]")
{
	SECTION("Construct from positiv int")
	{
		Number number(42);

		auto digits = number.getDigits();

		REQUIRE(digits.size() == 1);
		REQUIRE(digits[0] == 42);
		REQUIRE_FALSE(number.isNegative());
	}

	SECTION("Construct from negative int")
	{
		Number number(-42);

		auto digits = number.getDigits();

		REQUIRE(digits.size() == 1);
		REQUIRE(digits[0] == 42);
		REQUIRE(number.isNegative());
	}

	SECTION("Construct from zero")
	{
		Number number(0);

		auto digits = number.getDigits();

		REQUIRE(digits.empty());
		REQUIRE_FALSE(number.isNegative());
	}
}

TEST_CASE(
	"Number constructor from string representation (Base 10)"
	"[math][number]")
{
	SECTION("Construct from postiv decimal string")
	{
		Number number("12345");

		auto digits = number.getDigits();

		REQUIRE(digits.size() == 1);
		REQUIRE(digits[0] == 12345);
		REQUIRE_FALSE(number.isNegative());
	}

	SECTION("Construct from negative decimal string")
	{
		Number number("-12345");

		auto digits = number.getDigits();

		REQUIRE(digits.size() == 1);
		REQUIRE(digits[0] == 12345);
		REQUIRE(number.isNegative());
	}

	SECTION("Construct from zero string")
	{
		Number number("0");

		auto digits = number.getDigits();

		REQUIRE(digits.empty());
		REQUIRE_FALSE(number.isNegative());
	}

	SECTION("Construct with explicit base 10")
	{
		Number number("12345", 10);

		auto digits = number.getDigits();

		REQUIRE(digits.size() == 1);
		REQUIRE(digits[0] == 12345);
		REQUIRE_FALSE(number.isNegative());
	}

	SECTION("Construct from string with leading zeroes")
	{
		Number number("00012345");

		auto digits = number.getDigits();

		REQUIRE(digits.size() == 1);
		REQUIRE(digits[0] == 12345);
		REQUIRE_FALSE(number.isNegative());
	}

	SECTION("Construct with explicit positive sign")
	{
		Number number("+12345");

		auto digits = number.getDigits();

		REQUIRE(digits.size() == 1);
		REQUIRE(digits[0] == 12345);
		REQUIRE_FALSE(number.isNegative());
	}

	SECTION("Construct with explcit sign and leading zeroes")
	{
		Number number("+00012345");

		auto digits = number.getDigits();

		REQUIRE(digits.size() == 1);
		REQUIRE(digits[0] == 12345);
		REQUIRE_FALSE(number.isNegative());
	}

	SECTION("Construct with only leading zeroes")
	{
		Number number("0000");

		auto digits = number.getDigits();

		REQUIRE(digits.empty());
		REQUIRE_FALSE(number.isNegative());
	}

	SECTION("Construct with negative zero returns zero")
	{
		Number number("-0");

		auto digits = number.getDigits();

		REQUIRE(digits.empty());
		REQUIRE_FALSE(number.isNegative());
	}

	SECTION("Construct with empty string throws")
	{
		REQUIRE_THROWS_AS(Number(""), std::invalid_argument);
	}

	SECTION("Construct with invalid characters throws")
	{
		REQUIRE_THROWS_AS(Number("12a45"), std::invalid_argument);
		REQUIRE_THROWS_AS(Number("-12b45"), std::invalid_argument);
		REQUIRE_THROWS_AS(Number("12.45"), std::invalid_argument);
	}

	SECTION("Construct with only sign throws")
	{
		REQUIRE_THROWS_AS(Number("-"), std::invalid_argument);
		REQUIRE_THROWS_AS(Number("+"), std::invalid_argument);
	}

	SECTION("Construct with whitespace thrwos")
	{
		REQUIRE_THROWS_AS(Number(" 12345"), std::invalid_argument);
		REQUIRE_THROWS_AS(Number("12345 "), std::invalid_argument);
		REQUIRE_THROWS_AS(Number(" 12345 "), std::invalid_argument);
	}

	SECTION("Construct from 2^64 (one above max limb)")
	{
		// 2^64 = 18446744073709551616
		Number number("18446744073709551616");

		auto digits = number.getDigits();

		REQUIRE(digits.size() == 2);
		REQUIRE(digits[0] == 0);
		REQUIRE(digits[1] == 1);
		REQUIRE_FALSE(number.isNegative());
	}

	SECTION("Construct from 2^64 - 1 (max single limb)")
	{
		Number number("18446744073709551615");

		auto digits = number.getDigits();

		REQUIRE(digits.size() == 1);
		REQUIRE(digits[0] == 0xFFFFFFFFFFFFFFFFULL);
		REQUIRE_FALSE(number.isNegative());
	}

	SECTION("Construct from 2^128 - 1 (all limbs maxed)")
	{
		// 2^128 - 1 = 340282366920938463463374607431768211455
		Number number("340282366920938463463374607431768211455");

		auto digits = number.getDigits();

		REQUIRE_FALSE(number.isNegative());
		REQUIRE(digits.size() == 2);
		REQUIRE(digits[0] == 0xFFFFFFFFFFFFFFFFULL);
		REQUIRE(digits[1] == 0xFFFFFFFFFFFFFFFFULL);
	}

	SECTION("Construct from -2^128 + 1 (all limbs maxed, negative)")
	{
		// -2^128 + 1 = -340282366920938463463374607431768211455
		Number number("-340282366920938463463374607431768211455");

		auto digits = number.getDigits();

		REQUIRE(number.isNegative());
		REQUIRE(digits.size() == 2);
		REQUIRE(digits[0] == 0xFFFFFFFFFFFFFFFFULL);
		REQUIRE(digits[1] == 0xFFFFFFFFFFFFFFFFULL);
	}

	SECTION("Construct from -2^64 (negative, two limbs)")
	{
		// -2^64 = -18446744073709551616
		Number number("-18446744073709551616");

		auto digits = number.getDigits();

		REQUIRE(number.isNegative());
		REQUIRE(digits.size() == 2);
		REQUIRE(digits[0] == 0);
		REQUIRE(digits[1] == 1);
	}
}
