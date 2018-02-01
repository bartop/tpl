#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch.hpp>

#include <operation/mapped_values.hpp>

#include <vector>

using namespace std;
using namespace tpl;

TEST_CASE( "Keys", "[keys_test]" ) {
	SECTION("Test 1"){
		map<int, int> m{ {1, 2}, {3, 4}, {5, 6}, {7, 8}, {9, 10} };
		const auto vf = m | mapped_values;
		vector<int> result(vf.begin(), vf.end());
		vector<int> expected{ 2, 4, 6, 8, 10 };
		REQUIRE(expected == result);
	}
	SECTION("Test 2"){
		map<int, int> m{ };
		const auto vf = m | mapped_values;
		vector<int> result(vf.begin(), vf.end());
		vector<int> expected{ };
		REQUIRE(expected == result);
	}
	SECTION("Test 3"){
		map<int, int> m{ {2, 2}, {4, 4}, {6, 6}, {8, 8}, {11, 10} };
		const auto vf = m | mapped_values;
		vector<int> result(vf.begin(), vf.end());
		vector<int> expected{ 2, 4, 6, 8, 10 };
		REQUIRE(expected == result);
	}
}
