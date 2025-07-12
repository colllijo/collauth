#include "math/operations/Shifting.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("bitShiftRight", "[math][shifting]")
{
	SECTION("No shift")
	{
		std::vector<uint64_t> digits{1, 2, 3};

		auto result = bitShiftRight(digits, 0);

		REQUIRE(result == digits);
	}

	SECTION("Shift less than 64 bits")
	{
		std::vector<uint64_t> digits{0x123456789ABCDEF0, 0x0FEDCBA987654321};

		auto result = bitShiftRight(digits, 4);

		std::vector<uint64_t> expected = {0x1123456789abcdef, 0xfedcba98765432};
		REQUIRE(result.size() == 2);
		REQUIRE(result == expected);
	}

	SECTION("Shift more than 64 bits")
	{
		std::vector<uint64_t> digits{0x1, 0x2, 0x3};

		auto result = bitShiftRight(digits, 65);

		std::vector<uint64_t> expected = {0x8000000000000001, 0x1};
		REQUIRE(result.size() == 2);
		REQUIRE(result == expected);
	}

	SECTION("Shift all digits out")
	{
		std::vector<uint64_t> digits{1, 2, 3};

		auto result = bitShiftRight(digits, 192);

		REQUIRE(result.empty());
	}

	SECTION("Empty input")
	{
		std::vector<uint64_t> digits{};

		auto result = bitShiftRight(digits, 10);

		REQUIRE(result.empty());
	}
}

TEST_CASE("bitShiftLeft", "[math][shifting]")
{
	SECTION("No shift")
	{
		std::vector<uint64_t> digits{1, 2, 3};

		auto result = bitShiftLeft(digits, 0);

		REQUIRE(result == digits);
	}

	SECTION("Shift less than 64 bits")
	{
		std::vector<uint64_t> digits{0x123456789ABCDEF0, 0x0FEDCBA987654321};

		auto result = bitShiftLeft(digits, 4);

		std::vector<uint64_t> expected = {0x23456789abcdef00, 0xfedcba9876543211};
		REQUIRE(result.size() == 2);
		REQUIRE(result == expected);
	}

	SECTION("Shift more than 64 bits")
	{
		std::vector<uint64_t> digits{0x1, 0x2, 0x3};

		auto result = bitShiftLeft(digits, 65);

		std::vector<uint64_t> expected = {0x0, 0x2, 0x4, 0x6};
		REQUIRE(result.size() == 4);
		REQUIRE(result == expected);
	}

	SECTION("Empty input")
	{
		std::vector<uint64_t> digits{};

		auto result = bitShiftLeft(digits, 10);

		REQUIRE(result.empty());
	}
}

TEST_CASE("digitShiftRight", "[math][shifting]")
{
	SECTION("Shift less than size")
	{
		std::vector<uint64_t> digits{1, 2, 3, 4};

		auto result = digitShiftRight(digits, 2);

		std::vector<uint64_t> expected = {3, 4};
		REQUIRE(result.size() == 2);
		REQUIRE(result == expected);
	}

	SECTION("Shift equal to size")
	{
		std::vector<uint64_t> digits{1, 2, 3};

		auto result = digitShiftRight(digits, 3);

		REQUIRE(result.empty());
	}

	SECTION("Shift more than size")
	{
		std::vector<uint64_t> digits{1, 2};

		auto result = digitShiftRight(digits, 5);

		REQUIRE(result.empty());
	}
}

TEST_CASE("digitShiftLeft", "[math][shifting]")
{
	SECTION("Shift by zero")
	{
		std::vector<uint64_t> digits{1, 2, 3};

		auto result = digitShiftLeft(digits, 0);

		REQUIRE(result == digits);
	}

	SECTION("Shift by count")
	{
		std::vector<uint64_t> digits{1, 2};

		auto result = digitShiftLeft(digits, 3);

		std::vector<uint64_t> expected = {0, 0, 0, 1, 2};
		REQUIRE(result.size() == 5);
		REQUIRE(result == expected);
	}
}
