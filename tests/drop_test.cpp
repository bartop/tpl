#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch.hpp>

#include <tpl/operator/drop.hpp>

#include <string>
#include <algorithm>
#include <vector>
#include <iostream>

using namespace std;
using namespace tpl;

TEST_CASE( "Testing droping from sequence", "[drop_test]" ) {
	const vector<int> in = { 1, 2, 3, 4, 5, 6 };
	SECTION("Drop less than size") {
		const auto result = in | drop(1);
		const auto result2 = in | drop(3);
		const vector<int> out(result.begin(), result.end());
		const vector<int> out2(result2.begin(), result2.end());
		const vector<int> expected{2, 3, 4, 5, 6};
		const vector<int> expected2{4, 5 ,6};
		REQUIRE(out == expected);
		REQUIRE(out2 == expected2);
	}

	SECTION("Drop more than size") {
		const auto result = in | drop(8);
		const auto result2 = in | drop(10);
		const vector<int> out(result.begin(), result.end());
		const vector<int> out2(result2.begin(), result2.end());
		REQUIRE(out == vector<int>());
		REQUIRE(out2 == vector<int>());
	}
}
