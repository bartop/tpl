#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch.hpp>

#include <operation/cached.hpp>

#include <string>
#include <algorithm>
#include <vector>

using namespace std;
using namespace tpl;

TEST_CASE( "Testing caching", "[cache_test]" ) {
	vector<int> in = { 1, 2, 3, 4, 5, 6 };
	SECTION("Drop less than size") {
		auto result = in | cache;
		result.begin();
		auto in2 = in = { 1, 2, 3 };
		auto result2 = in2 | cache;
		const vector<int> out(result.begin(), result.end());
		const vector<int> out2(result2.begin(), result2.end());
		const vector<int> expected{1, 2, 3, 4, 5, 6};
		const vector<int> expected2{1, 2, 3};
		REQUIRE(out == expected);
		REQUIRE(out2 == expected2);
	}
}
