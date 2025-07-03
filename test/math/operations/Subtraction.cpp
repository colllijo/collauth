#include "math/operations/Subtraction.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Digit subtraction", "[math][subtraction]")
{
	SECTION("Subtract two vectors of equal length, no borrow")
	{
		std::vector<uint64_t> a{5, 7, 9};
		std::vector<uint64_t> b{1, 2, 3};

		auto result = subtractDigits(a, b);

		REQUIRE(result == std::vector<uint64_t>{4, 5, 6});
	}

	SECTION("Subtract larger vector from smaller vector, throws exception")
	{
		std::vector<uint64_t> a{0, 0, 1};
		std::vector<uint64_t> b{1, 2, 3};

		REQUIRE_THROWS_AS(subtractDigits(a, b), std::invalid_argument);
	}

	SECTION("Subtract with borrow propagating through all digits")
	{
		std::vector<uint64_t> a{0, 0, 1};
		std::vector<uint64_t> b{1};

		auto result = subtractDigits(a, b);

		// 0 - 1 = 2^64 - 1 with borrow, next digit: 0 - 0 - 1(borrow) = 2^64 - 1
		REQUIRE(result == std::vector<uint64_t>{0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL, 0});
	}

	SECTION("Subtract vector from empty vector")
	{
		std::vector<uint64_t> a{};
		std::vector<uint64_t> b{1, 2, 3};

		REQUIRE_THROWS_AS(subtractDigits(a, b), std::invalid_argument);
	}

	SECTION("Subtract zero vector from non-zero vector")
	{
		std::vector<uint64_t> a{5, 6, 7};
		std::vector<uint64_t> b{};

		auto result = subtractDigits(a, b);

		REQUIRE(result == std::vector<uint64_t>{5, 6, 7});
	}

	SECTION("Subtract empty vector from empty vector")
	{
		std::vector<uint64_t> a{};
		std::vector<uint64_t> b{};

		auto result = subtractDigits(a, b);

		REQUIRE(result == std::vector<uint64_t>{});
	}

	SECTION("Subtract with all zeros")
	{
		std::vector<uint64_t> a{0, 0, 0};
		std::vector<uint64_t> b{0, 0, 0};

		auto result = subtractDigits(a, b);

		REQUIRE(result == std::vector<uint64_t>{0, 0, 0});
	}
}
