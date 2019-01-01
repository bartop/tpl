#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch.hpp>

#include <tpl/meta/is_enumerable.hpp>
#include <tpl/operator/drop.hpp>
#include <tpl/operator/take.hpp>

#include <vector>

using namespace tpl::meta;

TEST_CASE( "Check created type traits", "[is_factory_test]" ) {
	REQUIRE(is_factory<tpl::drop_factory>::value == true);
	REQUIRE(is_factory<tpl::take_factory>::value == true);
	REQUIRE(is_factory<std::vector<int>>::value == false);
	REQUIRE(is_factory<int>::value == false);
}
