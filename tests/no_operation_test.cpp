#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch.hpp>

#include <tpl/operator/no_operation.hpp>

#include <string>
#include <algorithm>
#include <vector>
#include <iostream>

using namespace std;
using namespace tpl;

TEST_CASE( "Testing no operation operator", "[no_operation_test]" ) {
	const vector<int> in = { 1, 2, 3, 4, 5, 6 };
	const vector<int> in2 = { 2, 3, 4, 5, 6 };
	SECTION("Test simple no operation") {
		const auto result = in | no_operation;
		const auto result2 = in2 | no_operation;
		const vector<int> out(result.begin(), result.end());
		const vector<int> out2(result2.begin(), result2.end());
		REQUIRE(out == in);
		REQUIRE(out2 == in2);
	}
}
