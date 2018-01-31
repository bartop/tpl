#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch.hpp>

#include <operation/take.hpp>

#include <string>
#include <algorithm>
#include <vector>

using namespace std;
using namespace tpl::operation;

TEST_CASE( "Testing taking from sequence", "[take_test]" ) {
	const vector<int> in = { 1, 2, 3, 4, 5, 6 };
	SECTION("Take less than size") {
		const auto result = in | take(1);
		const auto result2 = in | take(3);
		const vector<int> out(result.begin(), result.end());
		const vector<int> out2(result2.begin(), result2.end());
		const vector<int> expected{1};
		const vector<int> expected2{1, 2, 3};
		REQUIRE(out == expected);
		REQUIRE(out2 == expected2);
	}

	SECTION("Take more than size") {
		const auto result = in | take(8);
		const auto result2 = in | take(10);
		const vector<int> out(result.begin(), result.end());
		const vector<int> out2(result2.begin(), result2.end());
		REQUIRE(out == in);
		REQUIRE(out2 == in);
	}
}
