#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch.hpp>

#include <meta/is_associative.hpp>

#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <list>

using namespace tpl::meta;

TEST_CASE( "Check created type traits", "[is_enumerable_test]" ) {
	REQUIRE(is_associative<char(&)[4]>::value == false);
	REQUIRE(is_associative<char>::value == false);
	REQUIRE(is_associative<std::vector<char>>::value == false);
	REQUIRE(is_associative<std::list<char>>::value == false);
	REQUIRE(is_associative<std::set<char>>::value == false);
	REQUIRE((is_associative<std::unordered_map<char, char>>::value) == true);
	REQUIRE((is_associative<std::map<char, char>>::value) == true);
	REQUIRE((is_associative<std::unordered_multimap<char, char>>::value) == true);
	REQUIRE((is_associative<std::multimap<char, char>>::value) == true);
}
