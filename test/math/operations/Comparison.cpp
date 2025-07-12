#include "math/operations/Comparison.hpp"

#include <catch2/catch_test_macros.hpp>

inline std::vector<uint64_t> makeDigits(std::initializer_list<uint64_t> digits) {
	return std::vector<uint64_t>(digits);
}

TEST_CASE("compareDigits: equality", "[math][comparison]") {
	SECTION("Equal vectors, no leading zeros") {
		std::vector<uint64_t> a{1, 2, 3};
		std::vector<uint64_t> b{1, 2, 3};
		REQUIRE(compareDigits(a, b) == std::strong_ordering::equal);
	}

	SECTION("Equal vectors, with leading zeros") {
		std::vector<uint64_t> a{1, 2, 3, 0, 0};
		std::vector<uint64_t> b{1, 2, 3};
		REQUIRE(compareDigits(a, b) == std::strong_ordering::equal);
	}
}

TEST_CASE("compareDigits: greater/less", "[math][comparison]") {
	SECTION("Greater by value") {
		std::vector<uint64_t> a{4, 5, 6};
		std::vector<uint64_t> b{4, 5, 2};
		REQUIRE(compareDigits(a, b) == std::strong_ordering::greater);
		REQUIRE(compareDigits(b, a) == std::strong_ordering::less);
	}

	SECTION("Greater by length") {
		std::vector<uint64_t> a{1, 2, 3, 4};
		std::vector<uint64_t> b{1, 2, 3};
		REQUIRE(compareDigits(a, b) == std::strong_ordering::greater);
		REQUIRE(compareDigits(b, a) == std::strong_ordering::less);
	}

	SECTION("Greater with leading zeros") {
		std::vector<uint64_t> a{1, 2, 4, 0, 0};
		std::vector<uint64_t> b{1, 2, 3};
		REQUIRE(compareDigits(a, b) == std::strong_ordering::greater);
		REQUIRE(compareDigits(b, a) == std::strong_ordering::less);
	}
}

TEST_CASE("compareDigits: edge cases", "[math][comparison]") {
	SECTION("Both empty") {
		std::vector<uint64_t> a{};
		std::vector<uint64_t> b{};
		REQUIRE(compareDigits(a, b) == std::strong_ordering::equal);
	}

	SECTION("One empty, one non-empty") {
		std::vector<uint64_t> a{};
		std::vector<uint64_t> b{0};
		REQUIRE(compareDigits(a, b) == std::strong_ordering::equal);

		std::vector<uint64_t> c{};
		std::vector<uint64_t> d{1};
		REQUIRE(compareDigits(c, d) == std::strong_ordering::less);
		REQUIRE(compareDigits(d, c) == std::strong_ordering::greater);
	}

	SECTION("All zeros") {
		std::vector<uint64_t> a{0, 0, 0};
		std::vector<uint64_t> b{0};
		REQUIRE(compareDigits(a, b) == std::strong_ordering::equal);
	}
}
