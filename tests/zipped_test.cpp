#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <tpl/operator/zipped.hpp>

#include <vector>
#include <list>

TEST_CASE( "Vector zipping", "[zipped_test]" ) {
	using namespace std;
	using namespace tpl;
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

TEST_CASE( "Compilation tests", "[zipped_test]" ) {
	using namespace std;

	SECTION("Iterator compilation"){
		tpl::zipped_iterator<vector<int>::iterator, vector<int>::iterator> 
			vectorAndvectorIterator;
		tpl::zipped_iterator<list<int>::iterator, vector<int>::iterator> 
			listAndvectorIterator;
		REQUIRE((tpl::zipped_iterator<vector<int>::iterator, vector<int>::iterator>() ==
					vectorAndvectorIterator));
		REQUIRE((tpl::zipped_iterator<list<int>::iterator, vector<int>::iterator>() ==
					listAndvectorIterator));
	}
}
