#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch.hpp>

#include <operation/zipped.hpp>

#include <vector>

TEST_CASE( "Vector zipping", "[zipped_test]" ) {
	using namespace std;
	using namespace tpl::operation;
	vector<int> v1{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	vector<int> v2{ 3, 4, 5, 6, 7, 8, 9, 10 };
	vector<int> v3{ 5, 6, 7, 8, 9, 10 };
	SECTION("1"){
		const auto vf = v1 | zip(v2);
		vector<std::pair<int, int>> result(vf.begin(), vf.end());
		vector<std::pair<int, int>> expected{{1, 3}, {2, 4}, {3, 5}, {4, 6}, {5, 7}, {6, 8}, {7, 9}, {8, 10} };
		REQUIRE(expected == result);
	}
	SECTION("2"){
		const auto vf = v1 | zip(v3);
		vector<std::pair<int, int>> result(vf.begin(), vf.end());
		vector<std::pair<int, int>> expected{{1, 5}, {2, 6}, {3, 7}, {4, 8}, {5, 9}, {6, 10} };
		REQUIRE(expected == result);
	}
	SECTION("3"){
		const auto vf = v2 | zip(v3);
		vector<std::pair<int, int>> result(vf.begin(), vf.end());
		vector<std::pair<int, int>> expected{{3, 5}, {4, 6}, {5, 7}, {6, 8}, {7, 9}, {8, 10} };
		REQUIRE(expected == result);
	}
}
