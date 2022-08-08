/* @author: Wang Liao
 * @date: 2022-7
 * @rational: Mainly use gdwg.constructors & gdwg.modifiers & gdwg.accessors
              to test different accessors.
 * @approach:
    * 1. Test is_node() to see if a node exists.
    * 2. Test empty() to see if the graph is empty.
    * 3. Test is_connected() to see if two nodes are connected.
    * 4. Test nodes() that get all nodes.
    * 5. Test weights() that get see all weights.
    * 6. Test find() that find a edge.
    * 7. Test connections() that get all edges connected to a node.
    * 8. Test exception.
 */

#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>

TEST_CASE("is_node()", "[gdwg.accessors]") {
	auto g = gdwg::graph<std::string, int>{"wang", "liao", "shi", "fan", "chen"};
	CHECK(g.is_node("wang"));
	CHECK(g.is_node("liao"));
	CHECK(g.is_node("shi"));
	CHECK(g.is_node("fan"));
	CHECK(g.is_node("chen"));
	CHECK_FALSE(g.is_node("wangliao"));
}

TEST_CASE("empty()", "[gdwg.accessors]") {
	auto g = gdwg::graph<std::string, int>{"wang", "liao", "shi", "fan", "chen"};
	CHECK_FALSE(g.empty());
	auto g1 = gdwg::graph<std::string, int>{};
	CHECK(g1.empty());
}

TEST_CASE("is_connected()", "[gdwg.accessors]") {
	auto g = gdwg::graph<std::string, int>{"wang", "liao", "shi", "fan", "chen"};
	g.insert_edge("wang", "liao", 1);
	g.insert_edge("wang", "shi", 2);
	g.insert_edge("wang", "fan", 3);
	g.insert_edge("wang", "chen", 4);

	CHECK(g.is_connected("wang", "liao"));
	CHECK(g.is_connected("wang", "shi"));
	CHECK(g.is_connected("wang", "fan"));
	CHECK(g.is_connected("wang", "chen"));
	CHECK_FALSE(g.is_connected("wang", "wang"));
}

TEST_CASE("nodes()", "[gdwg.accessors]") {
	auto g = gdwg::graph<std::string, int>{"wang", "liao", "shi", "fan", "chen"};
	auto nodes = g.nodes();
	auto expected_nodes = std::vector<std::string>{"chen", "fan", "liao", "shi", "wang"};
	CHECK(nodes == expected_nodes);
}

TEST_CASE("weights()", "[gdwg.accessors]") {
	auto g = gdwg::graph<std::string, int>{"wang", "liao", "shi", "fan", "chen"};
	g.insert_edge("wang", "liao", 1);
	g.insert_edge("wang", "shi", 2);
	g.insert_edge("wang", "fan", 3);
	g.insert_edge("wang", "chen", 4);
	auto weights = g.weights("wang", "liao");
	auto expected_weights = std::vector<int>{1};
	CHECK(weights == expected_weights);
}

TEST_CASE("find()", "[gdwg.accessors]") {
	auto g = gdwg::graph<std::string, int>{"wang", "liao", "shi", "fan", "chen"};
	g.insert_edge("wang", "liao", 1);
	g.insert_edge("wang", "shi", 2);
	g.insert_edge("wang", "fan", 3);
	g.insert_edge("wang", "chen", 4);
	auto edge = g.find("wang", "liao", 1);
	CHECK_FALSE(edge == g.end());
}

TEST_CASE("connections()", "[gdwg.accessors]") {
	auto g = gdwg::graph<std::string, int>{"wang", "liao", "shi", "fan", "chen"};
	g.insert_edge("wang", "liao", 1);
	g.insert_edge("wang", "shi", 2);
	g.insert_edge("wang", "fan", 3);
	g.insert_edge("wang", "chen", 4);
	auto connections = g.connections("wang");
	auto expected_connections = std::vector<std::string>{"chen", "fan", "liao", "shi"};
	CHECK(connections == expected_connections);
}

TEST_CASE("exception", "[gdwg.accessors]") {
	SECTION("is_connected()") {
		auto g = gdwg::graph<std::string, int>{"wang", "liao", "shi", "fan", "chen"};
		CHECK_THROWS_AS(g.is_connected("", "liao"), std::runtime_error);
		CHECK_THROWS_AS(g.is_connected("wang", ""), std::runtime_error);
		CHECK_THROWS_AS(g.is_connected("", ""), std::runtime_error);
	}

	SECTION("weights()") {
		auto g = gdwg::graph<std::string, int>{"wang", "liao", "shi", "fan", "chen"};
		CHECK_THROWS_AS(g.weights("", "liao"), std::runtime_error);
		CHECK_THROWS_AS(g.weights("wang", ""), std::runtime_error);
		CHECK_THROWS_AS(g.weights("", ""), std::runtime_error);
	}

	SECTION("connections()") {
		auto g = gdwg::graph<std::string, int>{"wang", "liao", "shi", "fan", "chen"};
		CHECK_THROWS_AS(g.connections(""), std::runtime_error);
		CHECK_THROWS_AS(g.connections("non-exist"), std::runtime_error);
	}
}