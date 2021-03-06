#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch.hpp>

#include <tpl/operator/filtered.hpp>

#include <vector>
#include <list>
#include <utility>
#include <forward_list>

TEST_CASE( "Vector filtering", "[filtered_test]" ) {
	using namespace std;
	using namespace tpl;
	vector<int> v{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	SECTION(" >= 1"){
		const auto vf = (v | filter([](const auto &i){ return i >= 1; }));
		vector<int> result(vf.begin(), vf.end());
		REQUIRE(v == result);
	}
	SECTION(" > 10"){
		const auto vf = (v | filter([](const auto &i){ return i > 10; }));
		vector<int> result(vf.begin(), vf.end());
		REQUIRE(vector<int>() == result);
	}
	SECTION(" > 5"){
		const auto vf = (v | filter([](const auto &i){ return i > 5; }));
		vector<int> result(vf.begin(), vf.end());
		REQUIRE((std::vector<int>{ 6, 7, 8, 9, 10 }) == result);
	}
}

TEST_CASE( "C array filtering", "[filtered_test]" ) {
	using namespace std;
	using namespace tpl;
	const int v[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	SECTION(" >= 1"){
		const auto vf = (v | filter([](const auto &i){ return i >= 1; }));
		vector<int> result(vf.begin(), vf.end());
		REQUIRE(std::equal(std::begin(v), std::end(v), result.begin()));
	}
	SECTION(" > 10"){
		const auto vf = (v | filter([](const auto &i){ return i > 10; }));
		vector<int> result(vf.begin(), vf.end());
		REQUIRE(vector<int>() == result);
	}
	SECTION(" > 5"){
		const auto vf = (v | filter([](const auto &i){ return i > 5; }));
		vector<int> result(vf.begin(), vf.end());
		REQUIRE((std::vector<int>{ 6, 7, 8, 9, 10 }) == result);
	}
}

TEST_CASE( "Vector filtering with using namespace tpl", "[filtered_test]" ) {
	using namespace std;
	vector<int> v{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	SECTION(" >= 1"){
		const auto vf = v | tpl::filter([](const auto &i){ return i >= 1; });
		vector<int> result(vf.begin(), vf.end());
		REQUIRE(v == result);
	}
	SECTION(" > 10"){
		const auto vf = v | tpl::filter([](const auto &i){ return i > 10; });
		vector<int> result(vf.begin(), vf.end());
		REQUIRE(vector<int>() == result);
	}
	SECTION(" > 5"){
		const auto vf = v | tpl::filter([](const auto &i){ return i > 5; });
		vector<int> result(vf.begin(), vf.end());
		REQUIRE((vector<int>{ 6, 7, 8, 9, 10 }) == result);
	}
}

TEST_CASE( "Checking filtered iterator in backward iteration", "[filtered_test]" ) {
	std::vector<int> v{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	SECTION(" >= 1"){
		const auto vf = v | tpl::filter([](const auto &i){ return i >= 1; });
		auto start = vf.begin();
		REQUIRE(*start == 1);
		REQUIRE(*(++start) == 2);
		REQUIRE(*(++start) == 3);
		REQUIRE(*(--start) == 2);
		REQUIRE(*(--start) == 1);
	}

	SECTION("is even"){
		const auto vf = v | tpl::filter([](const auto &i){ return i % 2 == 0; });
		auto start = vf.begin();
		REQUIRE(*(start) == 2);
		REQUIRE(*(++start) == 4);
		REQUIRE(*(++start) == 6);
		REQUIRE(*(--start) == 4);
		REQUIRE(*(--start) == 2);
	}

	SECTION("Checking postfix operators"){
		const auto vf = v | tpl::filter([](const auto &i){ return i >= 1; });
		auto start = vf.begin();
		REQUIRE(*(start++) == 1);
		REQUIRE(*(start++) == 2);
		REQUIRE(*(start--) == 3);
		REQUIRE(*(start--) == 2);
		REQUIRE(*(start) == 1);
	}
}

TEST_CASE( "Checking swap operation", "[filtered_test]" ) {
	std::vector<int> v{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	const auto vf = v | tpl::filter([](const auto &i){ return i == 1 || i == 3; });
	auto start = vf.begin();
	auto second = std::next(start);
	REQUIRE(*start == 1);
	REQUIRE(*second == 3);
	std::swap(start, second);
	REQUIRE(*start == 3);
	REQUIRE(*second == 1);
}

TEST_CASE( "Meta tests", "[filtered_test]" ) {
	SECTION("Iterator compilation"){
		tpl::filtering_iterator<std::vector<int>::iterator, std::vector<int>, bool (*)(int)> vectorAndFunctionPointerIterator;
		tpl::filtering_iterator<std::list<double>::iterator, std::vector<int>, bool (*)(double)> listAndFunctionPointerIterator;
		REQUIRE((tpl::filtering_iterator<std::vector<int>::iterator, std::vector<int>, bool (*)(int)>() ==
				vectorAndFunctionPointerIterator));
		REQUIRE((tpl::filtering_iterator<std::list<double>::iterator, std::vector<int>, bool (*)(double)>() ==
				listAndFunctionPointerIterator));
	}

	SECTION("Iterator category"){
		tpl::filtering_iterator<std::vector<int>::iterator, std::vector<int>, bool (*)(int)> it;
		tpl::filtering_iterator<std::forward_list<int>::iterator, std::forward_list<int>, bool (*)(int)> it2;
		REQUIRE((std::is_same<decltype(it)::iterator_category, std::bidirectional_iterator_tag>::value));
		REQUIRE((std::is_same<decltype(it2)::iterator_category, std::forward_iterator_tag>::value));
	}
}
