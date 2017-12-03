#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch.hpp>

#include <sink/count.hpp>

#include <vector>

using namespace std;
using namespace tpl;

TEST_CASE( "Count in initializer list", "[count_test]" ) {
	const auto v = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	SECTION(" >= 1"){
		const unsigned result = v | count([](const auto &i){ return i >= 1; });
		REQUIRE(10 == result);
	}
	SECTION(" > 10"){
		const unsigned result = (v | count([](const auto &i){ return i > 10; }));
		REQUIRE(0 == result);
	}
	SECTION(" > 5"){
		const unsigned result = (v | count([](const auto &i){ return i > 5; }));
		REQUIRE(5 == result);
	}
	SECTION(" < 5"){
		const unsigned result = (v | count([](const auto &i){ return i < 5; }));
		REQUIRE(4 == result);
	}
}
