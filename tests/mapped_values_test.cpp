#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch.hpp>

#include <tpl/operator/mapped_values.hpp>

#include <vector>

TEST_CASE( "Keys", "[keys_test]" ) {
	using namespace std;
	using namespace tpl;

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

TEST_CASE( "Checking swap operation", "[mapped_values_test]" ) {
	std::map<int, int> m{ {2, 2}, {4, 4} };
	const auto res = m | tpl::mapped_values;

	auto start = res.begin();
	auto second = std::next(start);
	REQUIRE(*start == 2);
	REQUIRE(*second == 4);
	std::swap(start, second);
	REQUIRE(*start == 4);
	REQUIRE(*second == 2);
}

TEST_CASE( "Compilation tests", "[mapped_values_test]" ) {
	using namespace std;

	SECTION("Iterator compilation"){
		tpl::mapped_values_iterator<map<int, int>::iterator> intMapIterator;
		tpl::mapped_values_iterator<map<string, string>::iterator> stringMapIterator;
		REQUIRE((tpl::mapped_values_iterator<map<int, int>::iterator>() == intMapIterator));
		REQUIRE((tpl::mapped_values_iterator<map<string, string>::iterator>() ==
				stringMapIterator));
	}
}
