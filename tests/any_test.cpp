#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch.hpp>

#include "any.hpp"

#include <vector>

using namespace std;
using namespace tpl;

TEST_CASE( "Any in vector", "[any_test]" ) {
	vector<int> v{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	SECTION(" >= 1"){
		const bool result = v | any([](const auto &i){ return i >= 1; });
		REQUIRE(true == result);
	}
	SECTION(" > 10"){
		const bool result = v | any([](const auto &i){ return i > 10; });
		REQUIRE(false == result);
	}
	SECTION(" > 5"){
		const bool result = v | any([](const auto &i){ return i > 5; });
		REQUIRE(true == result);
	}
	SECTION(" < 5"){
		const bool result = v | any([](const auto &i){ return i < 5; });
		REQUIRE(true == result);
	}
}
