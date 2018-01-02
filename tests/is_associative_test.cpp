#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch.hpp>

#include <meta/is_associative.hpp>
#include <operation/filtered.hpp>
#include <operation/transformed.hpp>

#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <list>
#include <functional>

using namespace tpl::meta;
using namespace tpl::operation;

TEST_CASE( "Check created type traits", "[is_associative_test]" ) {
	REQUIRE(is_associative<char(&)[4]>::value == false);
	REQUIRE(is_associative<char>::value == false);
	REQUIRE(is_associative<std::vector<char>>::value == false);
	REQUIRE(is_associative<std::list<char>>::value == false);
	REQUIRE(is_associative<std::set<char>>::value == false);
	REQUIRE((is_associative<std::unordered_map<char, char>>::value) == true);
	REQUIRE((is_associative<std::map<char, char>>::value) == true);
	REQUIRE((is_associative<std::unordered_multimap<char, char>>::value) == true);
	REQUIRE((is_associative<std::multimap<char, char>>::value) == true);

	using filter_type = decltype(filter(std::declval<std::function<bool(int)>>()));
	REQUIRE((is_associative<decltype(std::vector<int>() | std::declval<filter_type>())>::value) == false);
	REQUIRE((is_associative<decltype(std::set<int>() | std::declval<filter_type>())>::value) == false);

	using filter_type2 = decltype(filter(std::declval<std::function<bool(std::pair<int, int>)>>()));
	REQUIRE((is_associative<decltype(std::map<int, int>() | std::declval<filter_type2>())>::value) == true);

	REQUIRE((is_associative_element<std::pair<int, int>>::value) == true);
	REQUIRE((is_associative_element<std::pair<std::vector<int>, int>>::value) == true);
	REQUIRE((is_associative_element<std::pair<std::vector<int>, std::vector<int>>>::value) == true);
	REQUIRE((is_associative_element<std::vector<int>>::value) == false);
	REQUIRE((is_associative_element<int>::value) == false);

	using transform_type = decltype(transform(std::declval<std::function<std::pair<int, int>(int)>>()));
	REQUIRE((is_associative<decltype(std::vector<int>() | std::declval<transform_type>())>::value) == true);

	using transform_type2 = decltype(transform(std::declval<std::function<int(std::pair<int, int>)>>()));
	REQUIRE((is_associative<decltype(std::map<int, int>() | std::declval<transform_type2>())>::value) == false);
}
