#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch.hpp>

#include <flattened.hpp>

#include <vector>

using namespace std;
using namespace tpl;

TEST_CASE( "Vector of vectors flattening", "[flattened_test]" ) {
	vector<vector<int>> v;
	vector<int> expected;
	SECTION("Test 1"){
		v = { {1, 2, 3}, {4, 5} , {6, 7, 8, 9, 10} };
		expected = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

		const auto res = v | flatten;
		vector<int> result(res.begin(), res.end());
		REQUIRE(result == expected);
	}

	SECTION("Test 2"){
		v = { {}, {4, 5} , {} };
		expected = { 4, 5 };

		const auto res = v | flatten;
		vector<int> result(res.begin(), res.end());
		REQUIRE(result == expected);
	}

	SECTION("Empty test"){
		v = { {}, {} , {} };
		expected = { };

		const auto res = v | flatten;
		vector<int> result(res.begin(), res.end());
		REQUIRE(result == expected);
	}
}
