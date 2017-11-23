#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch.hpp>

#include <meta/is_enumerable.hpp>

#include <vector>

using namespace tpl::meta;

TEST_CASE( "Check created type traits", "[is_enumerable_test]" ) {
	REQUIRE(can_begin<char(&)[4]>::value == true);
	REQUIRE(can_end<char(&)[4]>::value == true);
	REQUIRE(can_begin<char>::value == false);
	REQUIRE(can_begin<char *>::value == false);
	REQUIRE(can_end<char>::value == false);
	REQUIRE(can_begin<std::vector<int>>::value == true);
	REQUIRE(can_end<std::vector<char>>::value == true);
	REQUIRE(can_begin<std::initializer_list<int>>::value == true);
	REQUIRE(can_end<std::initializer_list<char>>::value == true);

	REQUIRE((are_equality_comparable<int, char>::value) == true);
	REQUIRE((are_equality_comparable<int, int &>::value) == true);
	REQUIRE((are_equality_comparable<int, const unsigned &>::value) == true);
	REQUIRE((are_equality_comparable<int &, const unsigned &>::value) == true);
	REQUIRE((are_equality_comparable<int, unsigned &>::value) == true);
	REQUIRE((are_equality_comparable<bool, bool>::value) == true);
	REQUIRE((are_equality_comparable<int, bool>::value) == true);
	REQUIRE((are_equality_comparable<int, std::vector<int>>::value) == false);

	REQUIRE(is_enumerable<char(&)[4]>::value == true);
	REQUIRE(is_enumerable<char>::value == false);
	REQUIRE(is_enumerable<char *>::value == false);
	REQUIRE(is_enumerable<std::vector<int>>::value == true);
	REQUIRE(is_enumerable<std::initializer_list<int>>::value == true);
}
