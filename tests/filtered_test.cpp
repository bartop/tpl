#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch.hpp>

#include <tpl/operator/filtered.hpp>

#include <vector>
#include <list>

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

TEST_CASE( "Vector filtering without using namespace tpl", "[filtered_test]" ) {
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

TEST_CASE( "Compilation tests", "[filtered_test]" ) {
	SECTION("Iterator compilation"){
		tpl::filtering_iterator<std::vector<int>::iterator, bool (*)(int)> vectorAndFunctionPointerIterator;
		tpl::filtering_iterator<std::list<double>::iterator, bool (*)(double)> listAndFunctionPointerIterator;
		REQUIRE((tpl::filtering_iterator<std::vector<int>::iterator, bool (*)(int)>() ==
				vectorAndFunctionPointerIterator));
		REQUIRE((tpl::filtering_iterator<std::list<double>::iterator, bool (*)(double)>() ==
				listAndFunctionPointerIterator));
	}
}
