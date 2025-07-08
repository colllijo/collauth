#include "math/operations/Division.hpp"

#include <catch2/catch_test_macros.hpp>

#include "math/operations/Addition.hpp"
#include "math/operations/Multiplication.hpp"

inline std::vector<uint64_t> multiplyAdd(const std::vector<uint64_t>& divisor, const std::vector<uint64_t>& quotient, const std::vector<uint64_t>& add)
{
	return addDigits(multiplyDigits(divisor, quotient), add);
}

TEST_CASE("Digit division (single-limb)", "[math][division]")
{
	SECTION("Divide two small numbers")
	{
		std::vector<uint64_t> dividend{100};
		std::vector<uint64_t> divisor{7};

		auto [quotient, remainder] = divideDigits(dividend, divisor);

		std::vector<uint64_t> expected_quotient = { 0xe };
		std::vector<uint64_t> expected_remainder = { 0x2 };

		REQUIRE(quotient == expected_quotient);
		REQUIRE(remainder == expected_remainder);
		REQUIRE(multiplyAdd(divisor, quotient, remainder) == dividend);
	}

	SECTION("Divide by one")
	{
		std::vector<uint64_t> dividend{123456789};
		std::vector<uint64_t> divisor{1};

		auto [quotient, remainder] = divideDigits(dividend, divisor);

		REQUIRE(quotient == std::vector<uint64_t>{123456789});
		REQUIRE(remainder.empty());
		REQUIRE(multiplyAdd(divisor, quotient, remainder) == dividend);
	}

	SECTION("Divide zero by nonzero")
	{
		std::vector<uint64_t> dividend{0};
		std::vector<uint64_t> divisor{42};

		auto [quotient, remainder] = divideDigits(dividend, divisor);

		REQUIRE(quotient.empty());
		REQUIRE(remainder.empty());
	}

	SECTION("Divide by zero")
	{
		std::vector<uint64_t> dividend{123};
		std::vector<uint64_t> divisor{};

		REQUIRE_THROWS_AS(divideDigits(dividend, divisor), std::invalid_argument);
	}
}

TEST_CASE("Digit division (multi-limb)", "[math][division]")
{
	SECTION("Divide multi-limb by single-limb")
	{
		std::vector<uint64_t> dividend{0xFFFFFFFFFFFFFFFFULL, 1};
		std::vector<uint64_t> divisor{2};

		auto [quotient, remainder] = divideDigits(dividend, divisor);

		std::vector<uint64_t> expected_quotient = { 0xffffffffffffffff };
		std::vector<uint64_t> expected_remainder = { 0x1 };

		REQUIRE(quotient == expected_quotient);
		REQUIRE(remainder == expected_remainder);
		REQUIRE(multiplyAdd(divisor, quotient, remainder) == dividend);
	}

	SECTION("Divide multi-limb by multi-limb")
	{
		std::vector<uint64_t> dividend{0, 1};
		std::vector<uint64_t> divisor{3};

		auto [quotient, remainder] = divideDigits(dividend, divisor);

		REQUIRE(quotient == std::vector<uint64_t>{6148914691236517205ULL});
		REQUIRE(remainder == std::vector<uint64_t>{1});
		REQUIRE(multiplyAdd(divisor, quotient, remainder) == dividend);
	}

	SECTION("Divide by larger number")
	{
		std::vector<uint64_t> dividend{5};
		std::vector<uint64_t> divisor{10};

		auto [quotient, remainder] = divideDigits(dividend, divisor);

		REQUIRE(quotient.empty());
		REQUIRE(remainder == std::vector<uint64_t>{5});
		REQUIRE(multiplyAdd(divisor, quotient, remainder) == dividend);
	}
}

TEST_CASE("Digit division (edge cases and exceptions)", "[math][division]")
{
	SECTION("Divide by zero throws")
	{
		std::vector<uint64_t> dividend{123};
		std::vector<uint64_t> divisor{};

		REQUIRE_THROWS_AS(divideDigits(dividend, divisor), std::invalid_argument);
	}

	SECTION("Divide empty dividend")
	{
		std::vector<uint64_t> dividend{};
		std::vector<uint64_t> divisor{42};

		auto [quotient, remainder] = divideDigits(dividend, divisor);

		REQUIRE(quotient.empty());
		REQUIRE(remainder.empty());
		REQUIRE(multiplyAdd(divisor, quotient, remainder) == dividend);
	}
}
