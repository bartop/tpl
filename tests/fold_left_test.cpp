#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch.hpp>

#include <sink/fold_left.hpp>

#include <vector>

using namespace std;
using namespace tpl;

TEST_CASE( "Fold left initializer list", "[fold_left_test]" ) {
	const auto v = { 1, 2, 3, 4, 5 };
	int result = v | fold_left([](const auto &i, const auto &j){ return i+j;} , 0);
	REQUIRE(15 == result);
	result = v | fold_left([](const auto &i, const auto &j){ return i+j;} );
	REQUIRE(15 == result);
	result = v | fold_left([](const auto &i, const auto &j){ return i-j;} , 0);
	REQUIRE(-15 == result);
	result = v | fold_left([](const auto &i, const auto &j){ return i*j;} , 1);
	REQUIRE(120 == result);
	result = v | fold_left([](const auto &i, const auto &j){ return i*j;});
	REQUIRE(120 != result);
	result = v | fold_left([](const auto &i, const auto &j){ return i+j;} , 50);
	REQUIRE(65 == result);
	result = v | fold_left([](const auto &i, const auto &j){ return i+j;} , 150);
	REQUIRE(165 == result);
}
