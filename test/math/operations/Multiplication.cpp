#include "math/operations/Multiplication.hpp"

#include <catch2/catch_test_macros.hpp>

std::vector<uint64_t> removeLeadingZeros(const std::vector<uint64_t>& vec)
{
	return std::vector<uint64_t>(vec.begin(), std::find_if_not(vec.rbegin(), vec.rend(), [](uint64_t digit) { return digit == 0; }).base());
}

TEST_CASE("Digit multiplication (hardware multiplication)", "[math][multiplication]")
{
	SECTION("Multiply two small numbers")
	{
		std::vector<uint64_t> a{3};
		std::vector<uint64_t> b{7};

		auto result = hardwareMultiplication(a, b);
		result = removeLeadingZeros(result);

		REQUIRE(result == std::vector<uint64_t>{21});
	}

	SECTION("Multiply with explicit zero")
	{
		std::vector<uint64_t> a{0};
		std::vector<uint64_t> b{12345};

		auto result = hardwareMultiplication(a, b);
		result = removeLeadingZeros(result);

		REQUIRE(result == std::vector<uint64_t>{});
	}

	SECTION("Multiply two maximum 64-bit values")
	{
		std::vector<uint64_t> a{0xFFFFFFFFFFFFFFFFULL};
		std::vector<uint64_t> b{0xFFFFFFFFFFFFFFFFULL};

		auto result = hardwareMultiplication(a, b);
		result = removeLeadingZeros(result);

		REQUIRE(result == std::vector<uint64_t>{0x0000000000000001, 0xFFFFFFFFFFFFFFFE});
	}

	SECTION("Multiply empty vectors")
	{
		std::vector<uint64_t> a{};
		std::vector<uint64_t> b{};

		auto result = hardwareMultiplication(a, b);
		result = removeLeadingZeros(result);

		REQUIRE(result == std::vector<uint64_t>{});
	}

	SECTION("Multiply with one empty vector")
	{
		std::vector<uint64_t> a{123};
		std::vector<uint64_t> b{};

		auto result = hardwareMultiplication(a, b);
		result = removeLeadingZeros(result);

		REQUIRE(result == std::vector<uint64_t>{});

		std::vector<uint64_t> c{};
		std::vector<uint64_t> d{456};

		auto result2 = hardwareMultiplication(c, d);
		result = removeLeadingZeros(result);

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
		result = removeLeadingZeros(result);

		REQUIRE(result[0] == 408);
	}

	SECTION("Multiply with explicit zero")
	{
		std::vector<uint64_t> a{0};
		std::vector<uint64_t> b{987654321};

		auto result = longMultiplication(a, b);
		result = removeLeadingZeros(result);

		REQUIRE(result == std::vector<uint64_t>{});
	}

	SECTION("Multiply two maximum 64-bit values")
	{
		std::vector<uint64_t> a{0xFFFFFFFFFFFFFFFFULL};
		std::vector<uint64_t> b{0xFFFFFFFFFFFFFFFFULL};

		auto result = longMultiplication(a, b);
		result = removeLeadingZeros(result);

		REQUIRE(result == std::vector<uint64_t>{0x0000000000000001, 0xFFFFFFFFFFFFFFFE});
	}

	SECTION("Multiply two multi-limb numbers")
	{
		std::vector<uint64_t> a{0xFFFFFFFFFFFFFFFFULL, 2};
		std::vector<uint64_t> b{3, 4};

		auto result = longMultiplication(a, b);
		result = removeLeadingZeros(result);

		REQUIRE(result == std::vector<uint64_t>{0xFFFFFFFFFFFFFFFD, 0x0000000000000004, 0xC});
	}

	SECTION("Multiply with zero vector")
	{
		std::vector<uint64_t> a{123};
		std::vector<uint64_t> b{};

		auto result = longMultiplication(a, b);
		result = removeLeadingZeros(result);

		REQUIRE(result == std::vector<uint64_t>{});

		std::vector<uint64_t> c{};
		std::vector<uint64_t> d{456};

		auto result2 = longMultiplication(c, d);
		result = removeLeadingZeros(result);

		REQUIRE(result2 == std::vector<uint64_t>{});
	}

	SECTION("Multiply two empty vectors")
	{
		std::vector<uint64_t> a{};
		std::vector<uint64_t> b{};

		auto result = longMultiplication(a, b);
		result = removeLeadingZeros(result);

		REQUIRE(result.empty());
	}

	SECTION("Multiply two multi-limb numbers with large values and carry propagation")
	{
		std::vector<uint64_t> a{0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL, 0x2};
		std::vector<uint64_t> b{0xFFFFFFFFFFFFFFFFULL, 0x1, 0x3};

		auto result = longMultiplication(a, b);
		result = removeLeadingZeros(result);

		REQUIRE(result == std::vector<uint64_t>{0x0000000000000001, 0xFFFFFFFFFFFFFFFE, 0xFFFFFFFFFFFFFFF9, 0x0000000000000005, 0x9});
	}

	SECTION("Multiply two numbers with many limbs (stress test, all ones)")
	{
		const size_t N = 8;
		std::vector<uint64_t> a(N, 0xFFFFFFFFFFFFFFFFULL);
		std::vector<uint64_t> b(N, 0xFFFFFFFFFFFFFFFFULL);

		auto result = longMultiplication(a, b);
		result = removeLeadingZeros(result);

		REQUIRE(result == std::vector<uint64_t>{0x0000000000000001, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
												0x0000000000000000, 0x0000000000000000, 0xFFFFFFFFFFFFFFFE, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
												0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF});
	}

	SECTION("Multiply numbers with alternating zero and max limbs")
	{
		std::vector<uint64_t> a{0, 0xFFFFFFFFFFFFFFFFULL, 0, 0xFFFFFFFFFFFFFFFFULL};
		std::vector<uint64_t> b{0xFFFFFFFFFFFFFFFFULL, 0, 0xFFFFFFFFFFFFFFFFULL, 0};

		auto result = longMultiplication(a, b);
		result = removeLeadingZeros(result);

		REQUIRE(result ==
				std::vector<uint64_t>{0x0000000000000000, 0x0000000000000001, 0xFFFFFFFFFFFFFFFE, 0x0000000000000002, 0xFFFFFFFFFFFFFFFC, 0x0000000000000002, 0xFFFFFFFFFFFFFFFE});
	}
}

TEST_CASE("Digit multiplication (Karatsuba multiplication)", "[math][multiplication]")
{
	SECTION("Multiply two large multi-limb numbers")
	{
		std::vector<uint64_t> a{0xFFFFFFFFFFFFFFFFULL, 0x123456789ABCDEF0ULL, 0x0FEDCBA987654321ULL, 0x1111111111111111ULL};
		std::vector<uint64_t> b{0x2222222222222222ULL, 0x3333333333333333ULL, 0x4444444444444444ULL, 0x5555555555555555ULL};

		auto result = karatsubaMultiplication(a, b);
		result = removeLeadingZeros(result);

		// Compare with longMultiplication for correctness
		auto expected = longMultiplication(a, b);
		REQUIRE(result == expected);
	}

	SECTION("Multiply two numbers with alternating max and zero limbs")
	{
		std::vector<uint64_t> a{0xFFFFFFFFFFFFFFFFULL, 0, 0xFFFFFFFFFFFFFFFFULL, 0};
		std::vector<uint64_t> b{0x0, 0xFFFFFFFFFFFFFFFFULL, 0, 0xFFFFFFFFFFFFFFFFULL};

		auto result = karatsubaMultiplication(a, b);
		result = removeLeadingZeros(result);

		std::vector<uint64_t> expected{0x0000000000000000ULL, 0x0000000000000001ULL, 0xfffffffffffffffeULL, 0x0000000000000002ULL,
									   0xfffffffffffffffcULL, 0x0000000000000002ULL, 0xfffffffffffffffeULL};
		REQUIRE(result == expected);
	}

	SECTION("Multiply two numbers with all ones (stress test)")
	{
		const size_t N = 64;
		std::vector<uint64_t> a(N, 0xFFFFFFFFFFFFFFFFULL);
		std::vector<uint64_t> b(N, 0xFFFFFFFFFFFFFFFFULL);

		auto result = karatsubaMultiplication(a, b);
		result = removeLeadingZeros(result);

		std::vector<uint64_t> expected{
			0x0000000000000001ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL,
			0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL,
			0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL,
			0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL,
			0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL,
			0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL,
			0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL,
			0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL,
			0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL,
			0x0000000000000000ULL, 0xfffffffffffffffeULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL,
			0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL,
			0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL,
			0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL,
			0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL,
			0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL,
			0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL,
			0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL,
			0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL,
			0xffffffffffffffffULL, 0xffffffffffffffffULL};
		REQUIRE(result == expected);
	}

	SECTION("Multiply two numbers with random large values")
	{
		std::vector<uint64_t> a{0xd609f55e443fa95aULL, 0xcb32bb105956efe7ULL, 0x07c633ce7adf36bbULL, 0xda324a0332a9ff60ULL,
								0x82ee49e73d287754ULL, 0x8765112281283859ULL, 0x677b89ca81968e6aULL, 0xddbf0784946db8aaULL};
		std::vector<uint64_t> b{0xc3055d0b4d8c9230ULL, 0x6ca1dee157bf9260ULL, 0x8086ee39f3fdda2cULL, 0x7e5a749c014f7b13ULL,
								0x232e77ccc3faf266ULL, 0xd15c1157132332feULL, 0x71c6a7f63b9e2de9ULL, 0xb95ec82fa220f366ULL};

		auto result = karatsubaMultiplication(a, b);
		result = removeLeadingZeros(result);

		std::vector<uint64_t> expected{0x45e63637c9bd14e0ULL, 0xaa12ebf58348b29fULL, 0x88a91458773ce183ULL, 0xd6900b431465616aULL, 0xaf5da6c6ac308c23ULL, 0x842a0e8e9d1e7eddULL,
									   0x0fe8e50581c732d8ULL, 0xbeeb8e9d01d61f93ULL, 0xc3a161d31419b540ULL, 0xad785adaded41b61ULL, 0xaedfff584a675db2ULL, 0x2f2a5da914c30d63ULL,
									   0x6b9d5ef49a5e145cULL, 0x1e24a6ef77b4d8d9ULL, 0xd31187e9e54c5063ULL, 0xa09125fa12532d81ULL};
		REQUIRE(result == expected);
	}
}
