#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch.hpp>

#include <meta/enumerable_traits.hpp>
#include <filtered.hpp>
#include <transformed.hpp>

#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <list>
#include <functional>

using namespace tpl::meta;
using namespace tpl;


TEST_CASE( "Check created type traits", "[enumerable_traits_test]" ) {
	REQUIRE((std::is_same<char, typename enumerable_traits<char(&)[4]>::value_type>::value));
	REQUIRE((std::is_same<char, typename enumerable_traits<std::vector<char>>::value_type>::value));
	REQUIRE((std::is_same<char, typename enumerable_traits<std::list<char>>::value_type>::value));
	REQUIRE((std::is_same<char, typename enumerable_traits<std::set<char>>::value_type>::value));
	REQUIRE((std::is_same<std::pair<const char, char>, typename enumerable_traits<std::map<char, char>>::value_type>::value));
}
