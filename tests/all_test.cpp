#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch.hpp>

#include <sink/all.hpp>

#include <vector>

using namespace std;
using namespace tpl::sink;

TEST_CASE( "All in vector", "[all_test]" ) {
	const auto v = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	SECTION(" >= 1"){
		const bool result = (v | all([](const auto &i){ return i >= 1; }));
		REQUIRE(true == result);
	}
	SECTION(" > 10"){
		const bool result = (v | all([](const auto &i){ return i > 10; }));
		REQUIRE(false == result);
	}
	SECTION(" > 5"){
		const bool result = (v | all([](const auto &i){ return i > 5; }));
		REQUIRE(false == result);
	}
	SECTION(" < 5"){
		const bool result = (v | all([](const auto &i){ return i < 5; }));
		REQUIRE(false == result);
	}
}
