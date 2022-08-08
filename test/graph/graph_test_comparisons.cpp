/* @author: Wang Liao
 * @date: 2022-7
 * @rational: Mainly use gdwg.constructors & gdwg.modifiers & gdwg.accessors
              to test difference comparison.
 * @approach:
    * 1. Test comparison operator == with empty graph.
    * 2. Test comparison operator == with nodes.
    * 3. Test comparison operator == with nodes and edges.
    * 4. Test comparison operator != with empty graph.
    * 5. Test comparison operator != with nodes.
    * 6. Test comparison operator != with nodes and edges.
 */

#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>

TEST_CASE("comparison operator == with empty graph", "[gdwg.comparison]") {
	auto g1 = gdwg::graph<std::string, int>{};
	auto g2 = gdwg::graph<std::string, int>{};
	CHECK(g1 == g2);
}

TEST_CASE("comparison operator == with nodes", "[gdwg.comparison]") {
	auto g1 = gdwg::graph<std::string, int>{};
	auto g2 = gdwg::graph<std::string, int>{};
	g1.insert_node("wang");
	g2.insert_node("wang");
	CHECK(g1 == g2);
}

TEST_CASE("comparison operator == with nodes and edges", "[gdwg.comparison]") {
	auto g1 = gdwg::graph<std::string, int>{};
	auto g2 = gdwg::graph<std::string, int>{};
	g1.insert_node("wang");
	g1.insert_node("liao");
	g2.insert_node("wang");
	g2.insert_node("liao");
	g1.insert_edge("wang", "liao", 1);
	g2.insert_edge("wang", "liao", 1);
	CHECK(g1 == g2);
}

TEST_CASE("comparison operator != with empty graph", "[gdwg.comparison]") {
	auto g1 = gdwg::graph<std::string, int>{};
	auto g2 = gdwg::graph<std::string, int>{};
	g1.insert_node("wang");
	g2.insert_node("liao");
	CHECK(g1 != g2);
}

TEST_CASE("comparison operator != with nodes", "[gdwg.comparison]") {
	auto g1 = gdwg::graph<std::string, int>{};
	auto g2 = gdwg::graph<std::string, int>{};
	g1.insert_node("wang");
	g2.insert_node("liao");
	CHECK(g1 != g2);
}

TEST_CASE("comparison operator != with nodes and edges", "[gdwg.comparison]") {
	auto g1 = gdwg::graph<std::string, int>{};
	auto g2 = gdwg::graph<std::string, int>{};
	g1.insert_node("wang");
	g1.insert_node("liao");
	g2.insert_node("wang");
	g2.insert_node("liao");
	g1.insert_edge("wang", "liao", 1);
	g2.insert_edge("wang", "liao", 2);
	CHECK(g1 != g2);
}