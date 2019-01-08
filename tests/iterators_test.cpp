#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <tpl/meta/iterators.hpp>

#include <vector>
#include <list>

using namespace tpl::meta;

TEST_CASE( "Check created iterator traits", "[iterator_test]" ) {
	REQUIRE(is_random_access_iterator<std::vector<int>::iterator>::value);
	REQUIRE(is_bidirectional_iterator<std::list<int>::iterator>::value);
	REQUIRE((std::is_same<typename demote_to_bidirectional_tag<std::vector<int>::iterator>::type, std::bidirectional_iterator_tag>::value));
	REQUIRE((std::is_same<typename demote_to_bidirectional_tag<std::list<int>::iterator>::type, std::bidirectional_iterator_tag>::value));
	REQUIRE((std::is_same<typename demote_to_input_tag<std::vector<int>::iterator>::type, std::input_iterator_tag>::value));
	REQUIRE((std::is_same<typename demote_to_input_tag<std::list<int>::iterator>::type, std::input_iterator_tag>::value));
}
