#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch.hpp>

#include <generator/cycle.hpp>

#include <string>
#include <algorithm>
#include <vector>

using namespace std;
using namespace tpl;

TEST_CASE( "Testing cyclic sequence", "[cycle_test]" ) {
	SECTION("1") {
		const vector<int> a{1};
		const auto result = cycle(a);
		REQUIRE(1 == *result.begin());
		REQUIRE(1 == *(++result.begin()));
		std::vector<int> out;
		const std::vector<int> expected { 1, 1, 1, 1, 1};
		std::copy_n(result.begin(), 5, std::back_inserter(out));
		REQUIRE(out == expected );
	}

	SECTION("1, 2") {
		const auto a = {1, 2};
		const auto result = cycle(a);
		REQUIRE(1 == *result.begin());
		REQUIRE(2 == *(++result.begin()));
		REQUIRE(1 == *(++++result.begin()));
		std::vector<int> out;
		const std::vector<int> expected { 1, 2, 1, 2, 1};
		std::copy_n(result.begin(), 5, std::back_inserter(out));
		REQUIRE(out == expected );
	}

	SECTION("std::string(\"a\")") {
		const vector<string> a{"a"};
		const auto result = cycle(a);
		REQUIRE((*result.begin() == "a"s));
		REQUIRE((*(++result.begin()) == "a"s));
	}
}
