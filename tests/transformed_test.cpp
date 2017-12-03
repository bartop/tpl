#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch.hpp>

#include <operator/transformed.hpp>

#include <vector>

using namespace std;
using namespace tpl;

TEST_CASE( "Vector transforming", "[transformed_test]" ) {
	vector<int> v{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	SECTION(" >= 1"){
		const auto vf = (v | transform([](const auto &i){ return i >= 1; }));
		vector<bool> result(vf.begin(), vf.end());
		REQUIRE((vector<bool>(10, true))== result);
	}

	SECTION(" * 2"){
		const auto vf = (v | transform([](const auto &i){ return i * 2; }));
		vector<int> result(vf.begin(), vf.end());
		REQUIRE((vector<int>{ 2, 4, 6, 8, 10, 12, 14, 16, 18, 20})== result);
	}

	SECTION(" std::vector<int>(i) "){
		const auto vf = (vector<int>{ 1, 2 } | transform([](const auto &i){ return vector<int>{i}; }));
		vector<vector<int>> result(vf.begin(), vf.end());
		REQUIRE((vector<vector<int>>{ vector<int>{1}, vector<int>{2} }) == result);
	}
}
