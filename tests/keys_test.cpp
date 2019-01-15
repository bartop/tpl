#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch.hpp>

#include <tpl/operator/keys.hpp>

#include <vector>
#include <string>
#include <map>

TEST_CASE( "Keys", "[keys_test]" ) {
	using namespace std;
	using namespace tpl;

	SECTION("Test 1"){
		map<int, int> m{ {1, 2}, {3, 4}, {5, 6}, {7, 8}, {9, 10} };
		const auto vf = m | keys;
		vector<int> result(vf.begin(), vf.end());
		vector<int> expected{ 1, 3, 5, 7, 9 };
		REQUIRE(expected == result);
	}
	SECTION("Test 2"){
		map<int, int> m{ };
		const auto vf = m | keys;
		vector<int> result(vf.begin(), vf.end());
		vector<int> expected{ };
		REQUIRE(expected == result);
	}
	SECTION("Test 3"){
		map<int, int> m{ {2, 2}, {4, 4}, {6, 6}, {8, 8}, {11, 10} };
		const auto vf = m | keys;
		vector<int> result(vf.begin(), vf.end());
		vector<int> expected{ 2, 4, 6, 8, 11 };
		REQUIRE(expected == result);
	}
}

TEST_CASE( "Compilation tests", "[keys_test]" ) {
	using namespace std;

	SECTION("Iterator compilation"){
		tpl::keys_iterator<map<int, int>::iterator> intMapIterator;
		tpl::keys_iterator<map<string, string>::iterator> stringMapIterator;

		REQUIRE((tpl::keys_iterator<map<int, int>::iterator>() == intMapIterator));
		REQUIRE((tpl::keys_iterator<map<string, string>::iterator>() == stringMapIterator));
	}
}
