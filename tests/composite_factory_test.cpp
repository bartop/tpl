#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch.hpp>

#include <vector>

#include <tpl/operator/filtered.hpp>
#include <tpl/operator/transformed.hpp>
#include <tpl/operator/sorted.hpp>

using namespace tpl;

TEST_CASE( "Testing composite operations", "[composite_factory_test]" ) {
	const std::vector<int> in = { 1, 2, 3, 4, 5, 6 };
	const auto composite = 
		tpl::filter([](const auto &i){ return i > 2; }) |
	   	tpl::transform([](const auto &i){ return i * 2; })
;
	const auto result1 = in | composite;
	std::vector<int> out(result1.begin(), result1.end());
	std::vector<int> expected = { 6, 8, 10, 12 };
	REQUIRE(out == expected);

	const auto composite2 = 
		sort([](const auto &a, const auto &b) { return a > b; }) |
		filter([](const auto &i){ return i > 2; }) |
	   	transform([](const auto &i){ return i * 2; });
	const auto result2 = in | composite2;
	const auto e2 =  in |
		sort([](const auto &a, const auto &b) { return a > b; }) |
		filter([](const auto &i) { return i > 2; }) |
		transform([](const auto &i) { return i * 2; });
	const auto e3 = e2 |
		sort([](const auto &a, const auto &b) { return a > b; });
	expected.assign(e2.begin(), e2.end());
	out.assign(result2.begin(), result2.end());
	REQUIRE(out == expected);
}
