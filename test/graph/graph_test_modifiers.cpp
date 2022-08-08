/* @author: Wang Liao
 * @date: 2022-7
 * @rational: Mainly use gdwg.constructors & gdwg.modifiers & gdwg.accessors
              to test different modifiers.
 * @approach:
    * 1. Test insert_node() that add a node.
    * 2. Test insert_edge() that add an edge.
    * 3. Test replace_node()that replace the old data
    * 4. Test merge_replace_node() that replace the old data with new data
         and change the associated edges.
    * 5. Test erase_node() that erase a node.
    * 6. Test erase_edge() that erase an edge.
    * 7. Test clear that erase all nodes from the graph.
    * 8. Test exception.
 */

#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>

TEST_CASE("insert_node()", "[gdwg.modifiers]") {
	auto g = gdwg::graph<std::string, int>{"wang", "liao", "shi", "fan", "chen"};
	g.insert_node("wangliao");
	auto nodes = g.nodes();
	auto expected_nodes = std::vector<std::string>{"chen", "fan", "liao", "shi", "wang", "wangliao"};
	CHECK(nodes == expected_nodes);
}

TEST_CASE("insert_edge()", "[gdwg.modifiers]") {
	auto g = gdwg::graph<std::string, int>{"wang", "liao", "shi", "fan", "chen"};
	g.insert_edge("wang", "liao", 1);
	g.insert_edge("wang", "shi", 2);
	g.insert_edge("wang", "fan", 3);
	g.insert_edge("wang", "chen", 4);
	auto edges = g.connections("wang");
	auto expected_edges = std::vector<std::string>{"chen", "fan", "liao", "shi"};
	CHECK(edges == expected_edges);
}

TEST_CASE("merge_replace_node()", "[gdwg.modifiers]") {
	auto g = gdwg::graph<std::string, int>{"wang", "liao", "shi", "fan", "chen"};
	g.insert_edge("wang", "liao", 1);

	g.merge_replace_node("wang", "fan");
	auto nodes = g.nodes();
	auto expected_nodes = std::vector<std::string>{"chen", "fan", "liao", "shi"};
	CHECK(nodes == expected_nodes);

	CHECK(g.find("wang", "liao", 1) == g.end());
	CHECK_FALSE(g.find("fan", "liao", 1) == g.end());
}

TEST_CASE("erase_node()", "[gdwg.modifiers]") {
	auto g = gdwg::graph<std::string, int>{"wang", "liao", "shi", "fan", "chen"};
	g.erase_node("wang");
	auto nodes = g.nodes();
	auto expected_nodes = std::vector<std::string>{"chen", "fan", "liao", "shi"};
	CHECK(nodes == expected_nodes);
}

TEST_CASE("erase_edge()", "[gdwg.modifiers]") {
	auto g = gdwg::graph<std::string, int>{"wang", "liao", "shi", "fan", "chen"};
	g.insert_edge("wang", "liao", 1);
	g.insert_edge("wang", "shi", 2);
	g.insert_edge("wang", "fan", 3);
	g.insert_edge("wang", "chen", 4);

	SECTION("erase_edge() - v1") {
		g.erase_edge("wang", "liao", 1);
		auto edges = g.connections("wang");
		auto expected_edges = std::vector<std::string>{"chen", "fan", "shi"};
		CHECK(edges == expected_edges);
	}

	SECTION("erase_edge() - v2") {
		auto it = g.find("wang", "shi", 2);
		g.erase_edge(it);
		auto edges = g.connections("wang");
		auto expected_edges = std::vector<std::string>{"chen", "fan", "liao"};
		CHECK(edges == expected_edges);
	}

	SECTION("erase_edge() - v3") {
		auto it1 = g.find("wang", "liao", 1);
		auto it2 = g.find("wang", "shi", 2);
		g.erase_edge(it1, it2);
		auto edges = g.connections("wang");
		auto expected_edges = std::vector<std::string>{"chen", "fan", "shi"};
		CHECK(edges == expected_edges);
	}
}

TEST_CASE("clear()", "[gdwg.modifiers]") {
	auto g = gdwg::graph<std::string, int>{"wang", "liao", "shi", "fan", "chen"};
	g.clear();
	auto nodes = g.nodes();
	auto expected_nodes = std::vector<std::string>{};
	CHECK(nodes == expected_nodes);
}

TEST_CASE("exception", "[gdwg.modifiers]") {
	SECTION("insert_edge()") {
		auto g = gdwg::graph<std::string, int>{"wang", "liao", "shi", "fan", "chen"};
		CHECK_THROWS_AS(g.insert_edge("", "liao", 1), std::runtime_error);
		CHECK_THROWS_AS(g.insert_edge("wang", "", 1), std::runtime_error);
		CHECK_THROWS_AS(g.insert_edge("", "", 1), std::runtime_error);
	}

	SECTION("replace_node()") {
		auto g = gdwg::graph<std::string, int>{"wang", "liao", "shi", "fan", "chen"};
		CHECK_THROWS_AS(g.replace_node("", "liao"), std::runtime_error);
		CHECK_NOTHROW(g.replace_node("wang", ""));
		CHECK_THROWS_AS(g.replace_node("non", "non"), std::runtime_error);
	}

	SECTION("merfe_replace_node()") {
		auto g = gdwg::graph<std::string, int>{"wang", "liao", "shi", "fan", "chen"};
		CHECK_THROWS_AS(g.merge_replace_node("", "liao"), std::runtime_error);
		CHECK_THROWS_AS(g.merge_replace_node("wang", ""), std::runtime_error);
		CHECK_THROWS_AS(g.merge_replace_node("", ""), std::runtime_error);
	}

	SECTION("erase_edge()") {
		auto g = gdwg::graph<std::string, int>{"wang", "liao", "shi", "fan", "chen"};
		CHECK_THROWS_AS(g.erase_edge("", "liao", 1), std::runtime_error);
		CHECK_THROWS_AS(g.erase_edge("wang", "", 1), std::runtime_error);
		CHECK_THROWS_AS(g.erase_edge("", "", 1), std::runtime_error);
	}
}