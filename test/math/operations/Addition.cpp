#include "math/operations/Addition.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Digit addition", "[math][addition]")
{
	SECTION("Add two vectors of equal length, no carry")
	{
		std::vector<uint64_t> a{1, 2, 3};
		std::vector<uint64_t> b{4, 5, 6};

		auto result = addDigits(a, b);

		REQUIRE(result == std::vector<uint64_t>{5, 7, 9});
	}

	SECTION("Add two vectors of equal length, with carry")
	{
		std::vector<uint64_t> a{0xFFFFFFFFFFFFFFFFULL, 0, 1};
		std::vector<uint64_t> b{1, 2, 3};

		auto result = addDigits(a, b);

		REQUIRE(result == std::vector<uint64_t>{0, 3, 4});
	}

	SECTION("Add vectors of different lengths")
	{
		std::vector<uint64_t> a{1, 2};
		std::vector<uint64_t> b{3, 4, 5};

		auto result = addDigits(a, b);

		REQUIRE(result == std::vector<uint64_t>{4, 6, 5});
	}

	SECTION("Add with carry propagating through all digits")
	{
		std::vector<uint64_t> a{0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL};
		std::vector<uint64_t> b{1};

		auto result = addDigits(a, b);

		REQUIRE(result == std::vector<uint64_t>{0, 0, 1});
	}

	SECTION("Add empty vectors")
	{
		std::vector<uint64_t> a{};
		std::vector<uint64_t> b{};

		auto result = addDigits(a, b);

		REQUIRE(result == std::vector<uint64_t>{});
	}

	SECTION("Add vector to empty vector")
	{
		std::vector<uint64_t> a{1, 2, 3};
		std::vector<uint64_t> b{};

		auto result = addDigits(a, b);
		
		REQUIRE(result == std::vector<uint64_t>{1, 2, 3});
	}
}
