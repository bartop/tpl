#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch.hpp>

#include <tpl/operator/flattened.hpp>

#include <vector>
#include <list>


TEST_CASE( "Vector of vectors flattening", "[flattened_test]" ) {
	using namespace std;
	using namespace tpl;
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

TEST_CASE( "Checking swap operation", "[flattened_test]" ) {
	std::vector<std::vector<int>> v = { {}, {4, 5} , {} };

	const auto res = v | tpl::flatten;
	auto start = res.begin();
	auto second = std::next(start);
	REQUIRE(*start == 4);
	REQUIRE(*second == 5);
	std::swap(start, second);
	REQUIRE(*start == 5);
	REQUIRE(*second == 4);
}

TEST_CASE( "Compilation tests", "[flattened_test]" ) {
	using namespace std;
	SECTION("Iterator compilation"){
		tpl::flattening_iterator<vector<vector<int>>::iterator, vector<int>::iterator>
			nestedVectorsIterator;
		tpl::flattening_iterator<list<vector<int>>::iterator, vector<int>::iterator>
			listOfVectorsIterator;
		tpl::flattening_iterator<vector<list<int>>::iterator, list<int>::iterator>
			vectorOfListsIterator;
		REQUIRE((tpl::flattening_iterator<vector<vector<int>>::iterator, vector<int>::iterator>() ==
			nestedVectorsIterator));
		REQUIRE((tpl::flattening_iterator<list<vector<int>>::iterator, vector<int>::iterator>() ==
			listOfVectorsIterator));
		REQUIRE((tpl::flattening_iterator<vector<list<int>>::iterator, list<int>::iterator>() ==
			vectorOfListsIterator));
	}
}
