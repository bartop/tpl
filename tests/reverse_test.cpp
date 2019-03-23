#define CATCH_CONFIG_MAIN 
#include <catch.hpp>

#include <tpl/operator/reverse.hpp>

#include <vector>
#include <list>
#include <utility>
#include <forward_list>

TEST_CASE( "Vector reversing", "[reversed_test]" ) {
	using namespace tpl;
	std::vector<int> v{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	std::vector<int> expected = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };
	const auto vf = v | reverse;
	std::vector<int> result(vf.begin(), vf.end());
	REQUIRE(expected == result);
}

TEST_CASE( "C array reversing", "[reversed_test]" ) {
	using namespace tpl;
	const int v[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	std::vector<int> expected = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };
	const auto vf = v | reverse;
	std::vector<int> result(vf.begin(), vf.end());
	REQUIRE(expected == result);
}

TEST_CASE( "Vector reversing with using namespace tpl", "[reversed_test]" ) {
	using namespace std;
	vector<int> v{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	vector<int> expected = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };
	const auto vf = v | tpl::reverse;
	vector<int> result(vf.begin(), vf.end());
	REQUIRE(expected == result);
}

TEST_CASE( "Checking reversed iterator in backward iteration", "[reversed_test]" ) {
	std::vector<int> v{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	SECTION(" >= 1"){
		const auto vf = v | tpl::reverse;
		auto start = vf.begin();
		REQUIRE(*start == 10);
		REQUIRE(*(++start) == 9);
		REQUIRE(*(++start) == 8);
		REQUIRE(*(--start) == 9);
		REQUIRE(*(--start) == 10);
	}

	SECTION("Checking postfix operators"){
		const auto vf = v | tpl::reverse;
		auto start = vf.begin();
		REQUIRE(*(start++) == 10);
		REQUIRE(*(start++) == 9);
		REQUIRE(*(start--) == 8);
		REQUIRE(*(start--) == 9);
		REQUIRE(*(start) == 10);
	}
}

TEST_CASE( "Checking swap operation", "[reversed_test]" ) {
	std::vector<int> v{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	const auto vf = v | tpl::reverse;
	auto start = vf.begin();
	auto second = std::next(start);
	REQUIRE(*start == 10);
	REQUIRE(*second == 9);
	std::swap(start, second);
	REQUIRE(*start == 9);
	REQUIRE(*second == 10);
}
