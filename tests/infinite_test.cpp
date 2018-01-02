#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch.hpp>

#include <generator/infinite.hpp>

#include <string>
#include <algorithm>
#include <vector>

using namespace std;
using namespace tpl::generator;

TEST_CASE( "Testing infinite sequence", "[infinite_test]" ) {
	SECTION("1") {
		const auto result = infinite(1);
		REQUIRE(1 == *result.begin());
		REQUIRE(1 == *(++result.begin()));
		std::vector<int> out;
		const std::vector<int> expected { 1, 1, 1, 1, 1};
		std::copy_n(result.begin(), 5, std::back_inserter(out));
		REQUIRE(out == expected );
	}

	SECTION("std::string(\"a\")") {
		const auto result = infinite(std::string("a"));
		REQUIRE((*result.begin() == "a"s));
		REQUIRE((*(++result.begin()) == "a"s));
	}
}
