#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch.hpp>

#include <generator.hpp>

#include <string>
#include <algorithm>
#include <vector>

using namespace std;
using namespace tpl;
using namespace std::literals::string_literals;

TEST_CASE( "Testing generated sequence", "[generator_test]" ) {
	SECTION("1") {
		const auto result = generator([](const auto &i){ return i + 1; }, 1);
		REQUIRE(1 == *result.begin());
		REQUIRE(2 == *(++result.begin()));
		std::vector<int> out;
		const std::vector<int> expected { 1, 2, 3, 4, 5};
		std::copy_n(result.begin(), 5, std::back_inserter(out));
		REQUIRE(out == expected );
	}

	SECTION("std::string(\"a\")") {
		const auto result = generator([](const auto &s){ return s+"a"; }, std::string("a"));
		REQUIRE((*result.begin() == "a"s));
		REQUIRE((*(++result.begin()) == "aa"s));
	}
}
