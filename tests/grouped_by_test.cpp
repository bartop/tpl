#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch.hpp>

#include <tpl/operator/grouped_by.hpp>

#include <vector>
#include <unordered_map>
#include <algorithm>

TEST_CASE( "Vector grouping", "[grouped_by_test]" ) {
	std::vector<int> v{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	SECTION("i > 5"){
		const auto grouped = v | tpl::group_by([](const auto &i){ return i > 5; });
		std::unordered_map<bool, std::vector<int>> result(
			grouped.begin(),
			grouped.end()
		);
		std::unordered_map<bool, std::vector<int>> expected{ 
			{ true, { 6, 7, 8, 9, 10 } }, 
			{ false, { 1, 2, 3, 4, 5 } }
		};
		REQUIRE(expected == result);
	}

	SECTION("i"){
		auto lambda = [](const auto &i) { return i; };
		const auto grouped = v | tpl::group_by(lambda);
		std::unordered_map<int, std::vector<int>> result(
			grouped.begin(),
			grouped.end()
		);
		std::unordered_map<int, std::vector<int>> expected{ 
			{ 1, { 1 } }, 
			{ 2, { 2 } }, 
			{ 3, { 3 } }, 
			{ 4, { 4 } }, 
			{ 5, { 5 } }, 
			{ 6, { 6 } }, 
			{ 7, { 7 } }, 
			{ 8, { 8 } }, 
			{ 9, { 9 } }, 
			{ 10, { 10 } }
		};
		REQUIRE(expected == result);
	}
}
