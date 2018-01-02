#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch.hpp>

#include <operation/sorted.hpp>

#include <vector>

using namespace std;
using namespace tpl::operation;

TEST_CASE( "Vector sorting", "[transformed_test]" ) {
	vector<int> v{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	SECTION("i < j"){
		const auto vf = (v | sort([](const auto &i, const auto &j){ return i < j; }));
		vector<int> result(vf.begin(), vf.end());
		REQUIRE(v == result);
	}

	SECTION("i > j"){
		auto lambda = [](const auto &i, const auto &j){ return i > j; };
		const auto vf = (v | sort(lambda));
		vector<int> result(vf.begin(), vf.end());
		std::sort(v.begin(), v.end(), lambda);
		REQUIRE(v == result);
	}
}
