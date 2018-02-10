#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch.hpp>

#include <tpl/sink/copy_to.hpp>

#include <vector>

using namespace std;
using namespace tpl;

TEST_CASE( "Copying into a vector", "[copy_to_test]" ) {
	SECTION("Test 1"){
		vector<int> in {1, 3, 5, 7, 9};
		vector<int> out;
		in | copy_to(std::back_inserter(out));
		REQUIRE(in == out);
	}
	SECTION("Test 2"){
		vector<int> in;
		vector<int> out;
		in | copy_to(std::back_inserter(out));
		REQUIRE(in == out);
	}
	SECTION("Test 3"){
		vector<int> in(5, 10);
		vector<int> out;
		in | copy_to(std::back_inserter(out));
		REQUIRE(in == out);
	}
}
