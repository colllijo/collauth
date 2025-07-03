#include "math/operations/Multiplication.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Digit multiplication (hardware multiplication)", "[math][multiplication]")
{
	SECTION("Multiply two small numbers")
	{
		std::vector<uint64_t> a{3};
		std::vector<uint64_t> b{7};

		auto result = hardwareMultiplication(a, b);

		REQUIRE(result == std::vector<uint64_t>{21, 0});
	}

	SECTION("Multiply with explicit zero")
	{
		std::vector<uint64_t> a{0};
		std::vector<uint64_t> b{12345};

		auto result = hardwareMultiplication(a, b);

		REQUIRE(result == std::vector<uint64_t>{0, 0});
	}

	SECTION("Multiply two maximum 64-bit values")
	{
		std::vector<uint64_t> a{0xFFFFFFFFFFFFFFFFULL};
		std::vector<uint64_t> b{0xFFFFFFFFFFFFFFFFULL};

		auto result = hardwareMultiplication(a, b);

		REQUIRE(result == std::vector<uint64_t>{0x0000000000000001, 0xFFFFFFFFFFFFFFFE});
	}

	SECTION("Multiply empty vectors")
	{
		std::vector<uint64_t> a{};
		std::vector<uint64_t> b{};

		auto result = hardwareMultiplication(a, b);

		REQUIRE(result == std::vector<uint64_t>{});
	}

	SECTION("Multiply with one empty vector")
	{
		std::vector<uint64_t> a{123};
		std::vector<uint64_t> b{};

		auto result = hardwareMultiplication(a, b);

		REQUIRE(result == std::vector<uint64_t>{});

		std::vector<uint64_t> c{};
		std::vector<uint64_t> d{456};

		auto result2 = hardwareMultiplication(c, d);

		REQUIRE(result2 == std::vector<uint64_t>{});
	}
}

TEST_CASE("Digit multiplication (long multiplication)", "[math][multiplication]")
{
	SECTION("Multiply two small numbers")
	{
		std::vector<uint64_t> a{12};
		std::vector<uint64_t> b{34};

		auto result = longMultiplication(a, b);

		REQUIRE(result[0] == 408);
	}

	SECTION("Multiply with explicit zero")
	{
		std::vector<uint64_t> a{0};
		std::vector<uint64_t> b{987654321};

		auto result = longMultiplication(a, b);

		REQUIRE(result == std::vector<uint64_t>{0, 0});
	}

	SECTION("Multiply two maximum 64-bit values")
	{
		std::vector<uint64_t> a{0xFFFFFFFFFFFFFFFFULL};
		std::vector<uint64_t> b{0xFFFFFFFFFFFFFFFFULL};

		auto result = longMultiplication(a, b);

		REQUIRE(result == std::vector<uint64_t>{0x0000000000000001, 0xFFFFFFFFFFFFFFFE});
	}

	SECTION("Multiply two multi-limb numbers")
	{
		std::vector<uint64_t> a{0xFFFFFFFFFFFFFFFFULL, 2};
		std::vector<uint64_t> b{3, 4};

		auto result = longMultiplication(a, b);

		REQUIRE(result == std::vector<uint64_t>{0xFFFFFFFFFFFFFFFD, 0x0000000000000004, 0xC, 0x0});
	}

	SECTION("Multiply with zero vector")
	{
		std::vector<uint64_t> a{123};
		std::vector<uint64_t> b{};

		auto result = longMultiplication(a, b);

		REQUIRE(result == std::vector<uint64_t>{0});

		std::vector<uint64_t> c{};
		std::vector<uint64_t> d{456};

		auto result2 = longMultiplication(c, d);

		REQUIRE(result2 == std::vector<uint64_t>{0});
	}

	SECTION("Multiply two empty vectors")
	{
		std::vector<uint64_t> a{};
		std::vector<uint64_t> b{};

		auto result = longMultiplication(a, b);

		REQUIRE(result.empty());
	}

	SECTION("Multiply two multi-limb numbers with large values and carry propagation")
	{
		std::vector<uint64_t> a{0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL, 0x2};
		std::vector<uint64_t> b{0xFFFFFFFFFFFFFFFFULL, 0x1, 0x3};

		auto result = longMultiplication(a, b);

		REQUIRE(result == std::vector<uint64_t>{0x0000000000000001, 0xFFFFFFFFFFFFFFFE, 0xFFFFFFFFFFFFFFF9, 0x0000000000000005, 0x9, 0});
	}

	SECTION("Multiply two numbers with many limbs (stress test, all ones)")
	{
		const size_t N = 8;
		std::vector<uint64_t> a(N, 0xFFFFFFFFFFFFFFFFULL);
		std::vector<uint64_t> b(N, 0xFFFFFFFFFFFFFFFFULL);

		auto result = longMultiplication(a, b);

		REQUIRE(result == std::vector<uint64_t>{0x0000000000000001, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
												0x0000000000000000, 0x0000000000000000, 0xFFFFFFFFFFFFFFFE, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
												0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF});
	}

	SECTION("Multiply numbers with alternating zero and max limbs")
	{
		std::vector<uint64_t> a{0, 0xFFFFFFFFFFFFFFFFULL, 0, 0xFFFFFFFFFFFFFFFFULL};
		std::vector<uint64_t> b{0xFFFFFFFFFFFFFFFFULL, 0, 0xFFFFFFFFFFFFFFFFULL, 0};

		auto result = longMultiplication(a, b);

		REQUIRE(result ==
				std::vector<uint64_t>{0x0000000000000000, 0x0000000000000001, 0xFFFFFFFFFFFFFFFE, 0x0000000000000002, 0xFFFFFFFFFFFFFFFC, 0x0000000000000002, 0xFFFFFFFFFFFFFFFE, 0});
	}
}
