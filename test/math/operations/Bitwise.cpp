#include "math/operations/Bitwise.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("bitwiseAnd", "[math][bitwise]")
{
	SECTION("Equal length vectors")
	{
		std::vector<uint64_t> lhs{0xF0F0, 0xAAAA, 0x1234};
		std::vector<uint64_t> rhs{0x0F0F, 0x5555, 0xFFFF};

		auto result = bitwiseAnd(lhs, rhs);

		std::vector<uint64_t> expected{0xF0F0 & 0x0F0F, 0xAAAA & 0x5555, 0x1234 & 0xFFFF};
		REQUIRE(result == expected);
	}

	SECTION("Different length vectors")
	{
		std::vector<uint64_t> lhs{0xFF, 0xAA};
		std::vector<uint64_t> rhs{0x0F, 0x55, 0x99};

		auto result = bitwiseAnd(lhs, rhs);

		std::vector<uint64_t> expected{0xFF & 0x0F, 0xAA & 0x55};
		REQUIRE(result == expected);
	}

	SECTION("Empty vectors")
	{
		std::vector<uint64_t> lhs, rhs;

		auto result = bitwiseAnd(lhs, rhs);

		REQUIRE(result.empty());
	}
}

TEST_CASE("bitwiseOr", "[math][bitwise]")
{
	SECTION("Equal length vectors")
	{
		std::vector<uint64_t> lhs{0xF0F0, 0xAAAA, 0x1234};
		std::vector<uint64_t> rhs{0x0F0F, 0x5555, 0xFFFF};

		auto result = bitwiseOr(lhs, rhs);

		std::vector<uint64_t> expected{0xF0F0 | 0x0F0F, 0xAAAA | 0x5555, 0x1234 | 0xFFFF};
		REQUIRE(result == expected);
	}

	SECTION("Different length vectors")
	{
		std::vector<uint64_t> lhs{0xFF, 0xAA};
		std::vector<uint64_t> rhs{0x0F, 0x55, 0x99};

		auto result = bitwiseOr(lhs, rhs);

		std::vector<uint64_t> expected{0xFF | 0x0F, 0xAA | 0x55, 0x99};
		REQUIRE(result == expected);
	}

	SECTION("Empty vectors")
	{
		std::vector<uint64_t> lhs, rhs;

		auto result = bitwiseOr(lhs, rhs);

		REQUIRE(result.empty());
	}
}
