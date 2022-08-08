/* @author: Wang Liao
 * @date: 2022-7
 * @rational: Mainly use gdwg.constructors & gdwg.modifiers & gdwg.accessors
              to test iterator attributes.
 * @approach:
    * 1. Test iterator constructor.
    * 2. Test iterator traversal ++
    * 3. Test iterator traversal --
    * 4. Test iterator comparison
 */

#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>

TEST_CASE("iterator constructor", "[gdwg.iterator]") {
	SECTION("default constructor") {
		auto g = gdwg::graph<std::string, int>();
		CHECK(g.begin() == g.end());
	}

	SECTION("list constructor") {
		auto g = gdwg::graph<std::string, int>{"wang", "liao", "shi", "fan", "chen"};
		g.insert_edge("wang", "liao", 1);
		g.insert_edge("wang", "shi", 2);
		g.insert_edge("wang", "fan", 3);
		g.insert_edge("wang", "chen", 4);

		CHECK_FALSE(g.begin() == g.end());
	}
}

TEST_CASE("iterator traversal ++", "[gdwg.iterator]") {
	auto g = gdwg::graph<std::string, int>{"wang", "liao", "shi", "fan", "chen"};
	g.insert_edge("wang", "liao", 1);
	g.insert_edge("wang", "shi", 2);
	g.insert_edge("wang", "fan", 3);
	g.insert_edge("wang", "chen", 4);
	auto it = g.begin();
	CHECK((*it).from == "wang");
	CHECK((*it).to == "chen");
	CHECK((*it).weight == 4);
	++it;
	CHECK((*it).from == "wang");
	CHECK((*it).to == "fan");
	CHECK((*it).weight == 3);
	++it;
	CHECK((*it).from == "wang");
	CHECK((*it).to == "liao");
	CHECK((*it).weight == 1);
	++it;
	CHECK((*it).from == "wang");
	CHECK((*it).to == "shi");
	CHECK((*it).weight == 2);
	++it;
	CHECK(it == g.end());
}

TEST_CASE("iterator traversal --", "[gdwg.iterator]") {
	auto g = gdwg::graph<std::string, int>{"wang", "liao", "shi", "fan", "chen"};
	g.insert_edge("wang", "liao", 1);
	g.insert_edge("wang", "shi", 2);
	g.insert_edge("wang", "fan", 3);
	g.insert_edge("wang", "chen", 4);
	auto it = g.end();
	--it;
	CHECK((*it).from == "wang");
	CHECK((*it).to == "shi");
	CHECK((*it).weight == 2);
	--it;
	CHECK((*it).from == "wang");
	CHECK((*it).to == "liao");
	CHECK((*it).weight == 1);
	--it;
	CHECK((*it).from == "wang");
	CHECK((*it).to == "fan");
	CHECK((*it).weight == 3);
	--it;
	CHECK((*it).from == "wang");
	CHECK((*it).to == "chen");
	CHECK((*it).weight == 4);
	CHECK_FALSE(it == g.end());
}

TEST_CASE("iterator comparison", "[gdwg.iterator]") {
	auto g = gdwg::graph<std::string, int>{"wang", "liao", "shi", "fan", "chen"};
	g.insert_edge("wang", "liao", 1);
	g.insert_edge("wang", "shi", 2);
	g.insert_edge("wang", "fan", 3);
	g.insert_edge("wang", "chen", 4);
	auto it = g.begin();
	auto it2 = g.begin();
	CHECK(it == it2);
	++it;
	++it2;
	CHECK(it == it2);
}