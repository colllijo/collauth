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

TEST_CASE("Number constructor from string representation (Base 10)", "[math][number]")
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

TEST_CASE("Number constructor from string representation (Base 16)", "[math][number]")
{
	SECTION("Construct from positive hex string")
	{
		Number number("1a2b3c", 16);

		auto digits = number.getDigits();

		REQUIRE(digits.size() == 1);
		REQUIRE(digits[0] == 0x1a2b3c);
		REQUIRE_FALSE(number.isNegative());
	}

	SECTION("Construct from negative hex string")
	{
		Number number("-1A2B3C", 16);

		auto digits = number.getDigits();

		REQUIRE(digits.size() == 1);
		REQUIRE(digits[0] == 0x1A2B3C);
		REQUIRE(number.isNegative());
	}

	SECTION("Construct from hex string with leading zeroes")
	{
		Number number("0000deadbeef", 16);

		auto digits = number.getDigits();

		REQUIRE(digits.size() == 1);
		REQUIRE(digits[0] == 0xdeadbeef);
		REQUIRE_FALSE(number.isNegative());
	}

	SECTION("Construct from hex string with explicit positive sign")
	{
		Number number("+CAFEBABE", 16);

		auto digits = number.getDigits();

		REQUIRE(digits.size() == 1);
		REQUIRE(digits[0] == 0xCAFEBABE);
		REQUIRE_FALSE(number.isNegative());
	}

	SECTION("Construct from zero hex string")
	{
		Number number("0", 16);

		auto digits = number.getDigits();

		REQUIRE(digits.empty());
		REQUIRE_FALSE(number.isNegative());
	}

	SECTION("Construct from negative zero hex string")
	{
		Number number("-0", 16);

		auto digits = number.getDigits();

		REQUIRE(digits.empty());
		REQUIRE_FALSE(number.isNegative());
	}

	SECTION("Construct from 2^64 (one above max limb) in hex")
	{
		Number number("10000000000000000", 16);

		auto digits = number.getDigits();

		REQUIRE(digits.size() == 2);
		REQUIRE(digits[0] == 0x0);
		REQUIRE(digits[1] == 0x1);
		REQUIRE_FALSE(number.isNegative());
	}

	SECTION("Construct from 2^64 - 1 (max single limb) in hex")
	{
		Number number("FFFFFFFFFFFFFFFF", 16);

		auto digits = number.getDigits();

		REQUIRE(digits.size() == 1);
		REQUIRE(digits[0] == 0xFFFFFFFFFFFFFFFFULL);
		REQUIRE_FALSE(number.isNegative());
	}

	SECTION("Construct from 2^128 - 1 (all limbs maxed) in hex")
	{
		Number number("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF", 16);

		auto digits = number.getDigits();

		REQUIRE_FALSE(number.isNegative());
		REQUIRE(digits.size() == 2);
		REQUIRE(digits[0] == 0xFFFFFFFFFFFFFFFFULL);
		REQUIRE(digits[1] == 0xFFFFFFFFFFFFFFFFULL);
	}

	SECTION("Construct from -2^128 + 1 (all limbs maxed, negative) in hex")
	{
		Number number("-FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF", 16);

		auto digits = number.getDigits();

		REQUIRE(number.isNegative());
		REQUIRE(digits.size() == 2);
		REQUIRE(digits[0] == 0xFFFFFFFFFFFFFFFFULL);
		REQUIRE(digits[1] == 0xFFFFFFFFFFFFFFFFULL);
	}

	SECTION("Construct with invalid hex characters throws")
	{
		REQUIRE_THROWS_AS(Number("12g45", 16), std::invalid_argument);
		REQUIRE_THROWS_AS(Number("-12z45", 16), std::invalid_argument);
		REQUIRE_THROWS_AS(Number("12.45", 16), std::invalid_argument);
	}

	SECTION("Construct with only sign throws")
	{
		REQUIRE_THROWS_AS(Number("-", 16), std::invalid_argument);
		REQUIRE_THROWS_AS(Number("+", 16), std::invalid_argument);
	}

	SECTION("Construct with empty string throws")
	{
		REQUIRE_THROWS_AS(Number("", 16), std::invalid_argument);
	}
}

TEST_CASE("Number constructor from string representation (Base 2)", "[math][number]")
{
	SECTION("Construct from positive binary string")
	{
		Number number("101010", 2);

		auto digits = number.getDigits();

		REQUIRE(digits.size() == 1);
		REQUIRE(digits[0] == 0b101010);
		REQUIRE_FALSE(number.isNegative());
	}

	SECTION("Construct from negative binary string")
	{
		Number number("-101010", 2);

		auto digits = number.getDigits();

		REQUIRE(digits.size() == 1);
		REQUIRE(digits[0] == 0b101010);
		REQUIRE(number.isNegative());
	}

	SECTION("Construct from binary string with leading zeroes")
	{
		Number number("0000101010", 2);

		auto digits = number.getDigits();

		REQUIRE(digits.size() == 1);
		REQUIRE(digits[0] == 0b101010);
		REQUIRE_FALSE(number.isNegative());
	}

	SECTION("Construct from binary string with explicit positive sign")
	{
		Number number("+101010", 2);

		auto digits = number.getDigits();

		REQUIRE(digits.size() == 1);
		REQUIRE(digits[0] == 0b101010);
		REQUIRE_FALSE(number.isNegative());
	}

	SECTION("Construct from zero binary string")
	{
		Number number("0", 2);

		auto digits = number.getDigits();

		REQUIRE(digits.empty());
		REQUIRE_FALSE(number.isNegative());
	}

	SECTION("Construct from negative zero binary string")
	{
		Number number("-0", 2);

		auto digits = number.getDigits();

		REQUIRE(digits.empty());
		REQUIRE_FALSE(number.isNegative());
	}

	SECTION("Construct from 2^64 (one above max limb) in binary")
	{
		Number number("1" + std::string(64, '0'), 2);

		auto digits = number.getDigits();

		REQUIRE(digits.size() == 2);
		REQUIRE(digits[0] == 0x0);
		REQUIRE(digits[1] == 0x1);
		REQUIRE_FALSE(number.isNegative());
	}

	SECTION("Construct from 2^64 - 1 (max single limb) in binary")
	{
		Number number(std::string(64, '1'), 2);

		auto digits = number.getDigits();

		REQUIRE(digits.size() == 1);
		REQUIRE(digits[0] == 0xFFFFFFFFFFFFFFFFULL);
		REQUIRE_FALSE(number.isNegative());
	}

	SECTION("Construct from 2^128 - 1 (all limbs maxed) in binary")
	{
		Number number(std::string(128, '1'), 2);

		auto digits = number.getDigits();

		REQUIRE_FALSE(number.isNegative());
		REQUIRE(digits.size() == 2);
		REQUIRE(digits[0] == 0xFFFFFFFFFFFFFFFFULL);
		REQUIRE(digits[1] == 0xFFFFFFFFFFFFFFFFULL);
	}

	SECTION("Construct from -2^128 + 1 (all limbs maxed, negative) in binary")
	{
		Number number("-" + std::string(128, '1'), 2);

		auto digits = number.getDigits();

		REQUIRE(number.isNegative());
		REQUIRE(digits.size() == 2);
		REQUIRE(digits[0] == 0xFFFFFFFFFFFFFFFFULL);
		REQUIRE(digits[1] == 0xFFFFFFFFFFFFFFFFULL);
	}

	SECTION("Construct with invalid binary characters throws")
	{
		REQUIRE_THROWS_AS(Number("102010", 2), std::invalid_argument);
		REQUIRE_THROWS_AS(Number("-10a010", 2), std::invalid_argument);
		REQUIRE_THROWS_AS(Number("10.010", 2), std::invalid_argument);
	}

	SECTION("Construct with only sign throws")
	{
		REQUIRE_THROWS_AS(Number("-", 2), std::invalid_argument);
		REQUIRE_THROWS_AS(Number("+", 2), std::invalid_argument);
	}

	SECTION("Construct with empty string throws")
	{
		REQUIRE_THROWS_AS(Number("", 2), std::invalid_argument);
	}
}

TEST_CASE("Number basic arithmetic operations", "[math][number][arithmetic]")
{
	SECTION("Addition")
	{
		Number a(12345);
		Number b(67890);

		auto c = a + b;

		REQUIRE(c == Number(80235));
		REQUIRE_FALSE(c.isNegative());

		Number d(-12345);

		Number e = d + b;

		REQUIRE(e == Number(55545));
		REQUIRE_FALSE(e.isNegative());

		Number f = a + d;

		REQUIRE(f == Number(0));
		REQUIRE_FALSE(f.isNegative());

		Number zero(0);

		REQUIRE((a + zero) == a);
		REQUIRE((zero + a) == a);

		Number big("18446744073709551615");
		Number one(1);

		Number sum = big + one;

		REQUIRE(sum == Number("18446744073709551616"));
	}

	SECTION("Subtraction")
	{
		Number a(10000);
		Number b(2500);

		Number c = a - b;

		REQUIRE(c == Number(7500));
		REQUIRE_FALSE(c.isNegative());

		Number d = b - a;

		REQUIRE(d == Number(-7500));
		REQUIRE(d.isNegative());

		Number e(-10000);

		Number f = e - b;

		REQUIRE(f == Number(-12500));
		REQUIRE(f.isNegative());

		Number zero(0);

		REQUIRE((a - zero) == a);
		REQUIRE((zero - a) == Number(-10000));
		REQUIRE((zero - zero) == Number(0));

		Number big("18446744073709551615");
		REQUIRE((big - big) == Number(0));
	}

	SECTION("Multiplication")
	{
		Number a(123);
		Number b(456);
		Number c = a * b;
		REQUIRE(c == Number(56088));
		REQUIRE_FALSE(c.isNegative());

		Number d(-123);
		Number e = d * b;
		REQUIRE(e == Number(-56088));
		REQUIRE(e.isNegative());

		Number f = d * Number(-1);
		REQUIRE(f == Number(123));
		REQUIRE_FALSE(f.isNegative());

		Number zero(0);
		REQUIRE((a * zero) == Number(0));
		REQUIRE((zero * a) == Number(0));

		Number big("18446744073709551615");
		Number prod = big * big;
		REQUIRE(prod.getDigits().size() == 2);
		REQUIRE(prod.getDigits()[0] == 1);
		REQUIRE(prod.getDigits()[1] == 0xFFFFFFFFFFFFFFFEULL);
	}

	SECTION("Division")
	{
		Number a(56088);
		Number b(456);
		Number c = a / b;
		REQUIRE(c == Number(123));
		REQUIRE_FALSE(c.isNegative());

		Number d(-56088);
		Number e = d / b;
		REQUIRE(e == Number(-123));
		REQUIRE(e.isNegative());

		Number f = a / Number(-456);
		REQUIRE(f == Number(-123));
		REQUIRE(f.isNegative());

		Number zero(0);
		REQUIRE((zero / b) == Number(0));

		Number big("18446744073709551615");
		REQUIRE((big / big) == Number(1));

		REQUIRE_THROWS_AS(a / Number(0), std::invalid_argument);
		REQUIRE_THROWS_AS(zero / Number(0), std::invalid_argument);
	}

	SECTION("Modulo")
	{
		Number a(56088);
		Number b(456);
		Number c = a % b;
		REQUIRE(c == Number(0));
		REQUIRE_FALSE(c.isNegative());

		Number d(56089);
		Number e = d % b;
		REQUIRE(e == Number(1));
		REQUIRE_FALSE(e.isNegative());

		Number f(-56089);
		Number g = f % b;
		REQUIRE(g == Number(455));
		REQUIRE_FALSE(g.isNegative());

		Number zero(0);
		REQUIRE((zero % b) == Number(0));

		Number big("18446744073709551615");
		REQUIRE((big % big) == Number(0));

		REQUIRE_THROWS_AS(a % Number(0), std::invalid_argument);
		REQUIRE_THROWS_AS(zero % Number(0), std::invalid_argument);
	}

	SECTION("Edge cases: negative zero, large numbers, zero operands")
	{
		Number negZero("-0");
		Number zero(0);

		REQUIRE(negZero == zero);
		REQUIRE_FALSE(negZero.isNegative());

		Number big("340282366920938463463374607431768211455");	// 2^128 - 1
		Number small(1);

		REQUIRE((big + small).getDigits().size() == 3);
		REQUIRE((big - big) == Number(0));
		REQUIRE((big * zero) == Number(0));
		REQUIRE((zero * big) == Number(0));
	}
}

TEST_CASE("Number shifting operations", "[math][number][shifting]")
{
	SECTION("Shifting Operator")
	{
		Number num("F00D", 16);

		REQUIRE(num << 4 == Number("F00D0", 16));
		REQUIRE(num << 8 == Number("F00D00", 16));
		REQUIRE(num >> 4 == Number("F00", 16));
		REQUIRE(num >> 8 == Number("F0", 16));
	}

	SECTION("Shifting bits")
	{
		Number num("F00D", 16);

		REQUIRE(num.leftShift(4) == Number("F00D0", 16));
		REQUIRE(num.leftShift(4) == Number("F00D00", 16));

		num = Number("F00D", 16);

		REQUIRE(num.rightShift(4) == Number("F00", 16));
		REQUIRE(num.rightShift(4) == Number("F0", 16));
	}

	SECTION("Shifting digits")
	{
		Number num("F00D", 16);

		REQUIRE(num.leftShiftDigit(1) == Number("F00D0000000000000000", 16));

		num = Number("F00D", 16);

		REQUIRE(num.rightShiftDigit(1) == Number("0", 16));
	}
}


TEST_CASE("Number comparison operations", "[math][number][comparison]")
{
	SECTION("Equality and Comparison")
	{
		Number a(12345);
		Number b(12345);
		Number c(67890);
		Number d(-12345);

		REQUIRE(a == b);
		REQUIRE_FALSE(a == c);
		REQUIRE_FALSE(a == d);

		REQUIRE(a < c);
		REQUIRE(a > d);
		REQUIRE(c > a);
		REQUIRE(d < a);

		REQUIRE((a <=> b) == std::strong_ordering::equal);
		REQUIRE((a <=> c) == std::strong_ordering::less);
		REQUIRE((c <=> a) == std::strong_ordering::greater);
		REQUIRE((a <=> d) == std::strong_ordering::greater);
	}
}

TEST_CASE("Number bitwise operations", "[math][number][bitwise]")
{
	SECTION("Bitwise AND")
	{
		Number a(0b1100);
		Number b(0b1010);
		Number c = a & b;

		REQUIRE(c == Number(0b1000));
		REQUIRE_FALSE(c.isNegative());
	}

	SECTION("Bitwise OR")
	{
		Number a(0b1100);
		Number b(0b1010);
		Number c = a | b;

		REQUIRE(c == Number(0b1110));
		REQUIRE_FALSE(c.isNegative());
	}
}

TEST_CASE("Number advanced arithmetic operations", "[math][number][advanced]")
{
	SECTION("Power (pow)")
	{
		Number base(2);
		Number exp(10);
		Number result = base.pow(exp);
		REQUIRE(result == Number(1024));

		REQUIRE(Number::pow(Number(3), Number(4)) == Number(81));
		REQUIRE(Number::pow(Number(5), Number(0)) == Number(1));
	}

	SECTION("Modular Power (modPow)")
	{
		Number base(2);
		Number exp(10);
		Number mod(1000);
		Number result = base.modPow(exp, mod);
		REQUIRE(result == Number(24));

		REQUIRE(Number::modPow(Number(3), Number(4), Number(5)) == Number(1));
	}

	SECTION("GCD")
	{
		Number a(48);
		Number b(18);
		REQUIRE(a.gcd(b) == Number(6));
		REQUIRE(Number::gcd(Number(270), Number(192)) == Number(6));
	}

	SECTION("Extended GCD")
	{
		Number a(30);
		Number b(20);
		auto [g, x, y] = a.extendedGCD(b);
		REQUIRE(g == Number(10));
		REQUIRE(a * x + b * y == g);

		auto [g2, x2, y2] = Number::extendedGCD(Number(99), Number(78));
		REQUIRE(g2 == Number(3));
		REQUIRE(Number(99) * x2 + Number(78) * y2 == g2);
	}

	SECTION("Modular Inverse")
	{
		Number a(3);
		Number mod(11);
		REQUIRE(a.modInverse(mod) == Number(4));
		REQUIRE(Number::modInverse(Number(10), Number(17)) == Number(12));
		REQUIRE_THROWS_AS(Number(2).modInverse(Number(4)), std::invalid_argument);
	}
}
