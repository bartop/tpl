#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch.hpp>

#include <common/composite_factory.hpp>
#include <operation/filtered.hpp>
#include <operation/transformed.hpp>
#include <operation/sorted.hpp>

#include <string>
#include <algorithm>
#include <vector>

using namespace std;
using namespace tpl;

TEST_CASE( "Testing composite operations", "[composite_factory_test]" ) {
	const vector<int> in = { 1, 2, 3, 4, 5, 6 };
	const auto composite =
		filter([](const auto &i){ return i > 2; }) |
	   	transform([](const auto &i){ return i * 2; });
	const auto result1 = in | composite;
	vector<int> out(result1.begin(), result1.end());
	vector<int> expected = { 6, 8, 10, 12 };
	REQUIRE(out == expected );


	const auto composite2 =
		sort([](const auto &a, const auto &b) { return a > b; } ) |
		filter([](const auto &i){ return i > 2; }) |
	   	transform([](const auto &i){ return i * 2; });
	const auto result2 = in | composite2;
	const auto e2 =  in |
		sort([](const auto &a, const auto &b) { return a > b; } ) |
		filter([](const auto &i){ return i > 2; }) |
	   	transform([](const auto &i){ return i * 2; });
	expected.assign(e2.begin(), e2.end());
	out.assign(result2.begin(), result2.end());
	REQUIRE(out == expected);
}
