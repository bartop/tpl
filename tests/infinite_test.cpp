#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch.hpp>

#include <infinite.hpp>

#include <string>

using namespace std;
using namespace tpl;

TEST_CASE( "Testing infinite sequence", "[infinite_test]" ) {
	SECTION("1") {
		const auto result = infinite(1);
		SECTION("first"){
			REQUIRE(1 == *result.begin());
		}
		SECTION("second"){
			REQUIRE(1 == *(++result.begin()));
		}
	}

	SECTION("std::string(\"a\")") {
		const auto result = infinite(std::string("a"));
		SECTION("first"){
			REQUIRE((*result.begin() == "a"s));
		}
		SECTION("second"){
			REQUIRE((*(++result.begin()) == "a"s));
		}
	}
}
