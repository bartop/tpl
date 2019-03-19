#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <tpl/operator/transformed.hpp>
#include <tpl/meta/iterators.hpp>

#include <vector>
#include <list>
#include <iterator>

TEST_CASE( "Compilation tests", "[transformed_test]" ) {
	SECTION("Iterator compilation"){
		tpl::transforming_iterator<std::vector<int>::iterator, bool (*)(int)> vectorAndFunctionPointerIterator;
		tpl::transforming_iterator<std::list<double>::iterator, bool (*)(double)> listAndFunctionPointerIterator;
		REQUIRE((tpl::transforming_iterator<std::vector<int>::iterator, bool (*)(int)>() ==
				vectorAndFunctionPointerIterator));
		REQUIRE((tpl::transforming_iterator<std::list<double>::iterator, bool (*)(double)>() ==
				listAndFunctionPointerIterator));
	}
}

TEST_CASE( "Vector transforming", "[transformed_test]" ) {
	using namespace std;
	using namespace tpl;
	vector<int> v{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	SECTION(" >= 1"){
		const auto vf = (v | transform([](const auto &i){ return i >= 1; }));
		vector<bool> result(vf.begin(), vf.end());
		REQUIRE((vector<bool>(10, true))== result);
	}

	SECTION(" * 2"){
		const auto vf = (v | transform([](const auto &i){ return i * 2; }));
		vector<int> result(vf.begin(), vf.end());
		REQUIRE((vector<int>{ 2, 4, 6, 8, 10, 12, 14, 16, 18, 20})== result);
	}

	SECTION("std::vector<int>(i)"){
		const auto vf = (vector<int>{ 1, 2 } | transform([](const auto &i){ return vector<int>{i}; }));
		vector<vector<int>> result(vf.begin(), vf.end());
		REQUIRE((vector<vector<int>>{ vector<int>{1}, vector<int>{2} }) == result);
	}
}

TEST_CASE( "Reverse iterators", "[transformed_test]" ) {
	using namespace std;
	using namespace tpl;
	vector<int> v{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	SECTION(" >= 1"){
		const auto vf = (v | transform([](const auto &i){ return i >= 1; }));
		vector<bool> result(meta::make_reverse_iterator(vf.end()), meta::make_reverse_iterator(vf.begin()));
		REQUIRE((vector<bool>(10, true))== result);
	}

	SECTION(" * 2"){
		const auto vf = (v | transform([](const auto &i){ return i * 2; }));
		vector<int> result(meta::make_reverse_iterator(vf.end()), meta::make_reverse_iterator(vf.begin()));
		REQUIRE((vector<int>{ 20, 18, 16, 14, 12, 10, 8, 6, 4, 2 })== result);
	}
}

TEST_CASE( "Checking swap operation", "[transformed_test]" ) {
	std::vector<int> v{ 1, 2 };
	const auto res = (v | tpl::transform([](const auto &i){ return i <= 1; }));

	auto start = res.begin();
	auto second = std::next(start);
	REQUIRE(*start == true);
	REQUIRE(*second == false);
	std::swap(start, second);
	REQUIRE(*start == false);
	REQUIRE(*second == true);
}
