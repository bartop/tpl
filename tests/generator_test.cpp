#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch.hpp>

#include <tpl/generator/generator.hpp>

#include <string>
#include <algorithm>
#include <vector>
#include <iterator>
#include <functional>

TEST_CASE( "Testing generated sequence", "[generator_test]" ) {
	using namespace std::string_literals;
	using namespace std;
	using namespace tpl;

	SECTION("1") {
		const auto result = generator([](const auto &i){ return i + 1; }, 1);
		REQUIRE(1 == *result.begin());
		REQUIRE(2 == *(++result.begin()));
		std::vector<int> out;
		const std::vector<int> expected { 1, 2, 3, 4, 5};
		std::copy_n(result.begin(), 5, std::back_inserter(out));
		REQUIRE(out == expected);
	}

	SECTION("std::string(\"a\")") {
		const auto result = generator([](const auto &s){ return s+"a"; }, std::string("a"));
		REQUIRE((*result.begin() == "a"s));
		REQUIRE((*(++result.begin()) == "aa"s));
	}

	SECTION("Default iterator constructor compilation") {
		tpl::generating_iterator<std::function<int(int)>, int> intFunctionIterator;
		tpl::generating_iterator<std::function<std::string(std::string)>, int> stringFunctionIterator;
		REQUIRE((tpl::generating_iterator<std::function<int(int)>, int>() != intFunctionIterator));
		REQUIRE((tpl::generating_iterator<std::function<std::string(std::string)>, int>() !=
				stringFunctionIterator));
	}
}
