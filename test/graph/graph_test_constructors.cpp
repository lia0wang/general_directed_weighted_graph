/* @author: Wang Liao
 * @date: 2022-7
 * @rational: Mainly use gdwg.constructors & gdwg.modifiers & gdwg.accessors
              to test different constructors.
 * @approach:
    * 1. Test default constructor.
    * 2. Test initializer list constructor.
    * 3. Test iterator constructor.
    * 4. Test copy constructor.
    * 5. Test move constructor.
    * 6. Test copy assignment operator.
    * 7. Test move assignment operator.
 */

#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>

TEST_CASE("default constructor", "[gdwg.constructors]") {
	auto g = gdwg::graph<int, std::string>{};
	CHECK(g.empty());

	g.insert_node(-1);
	g.insert_node(0);
	g.insert_node(1);
	g.insert_edge(-1, 0, "a");
	CHECK_FALSE(g.empty());

	auto nodes = g.nodes();
	auto expected_nodes = std::vector<int>{-1, 0, 1};
	CHECK(nodes == expected_nodes);

	auto weights = g.weights(-1, 0);
	auto expected_weights = std::vector<std::string>{"a"};
	CHECK(weights == expected_weights);

	auto edges = g.connections(-1);
	CHECK(g.is_connected(-1, 0));
}

TEST_CASE("initializer list constructor", "[gdwg.constructors]") {
	auto v1 = std::vector<int>{-1, 0, 1};
	auto v2 = std::vector<int>{2, 3, 4};
	auto v3 = std::vector<int>{5, 6, 7};
	auto g1 = gdwg::graph<int, std::string>{-1, 0, 1};
	auto g2 = gdwg::graph<std::string, std::string>{"a", "b", "c"};
	auto g3 = gdwg::graph<std::vector<int>, std::string>{v1, v2, v3};

	SECTION("type int") {
		CHECK(g1.is_node(-1));
		CHECK(g1.is_node(0));
		CHECK(g1.is_node(1));
	}

	SECTION("type string") {
		CHECK(g2.is_node("a"));
		CHECK(g2.is_node("b"));
		CHECK(g2.is_node("c"));
	}

	SECTION("type vector<int>") {
		CHECK(g3.is_node(v1));
		CHECK(g3.is_node(v2));
		CHECK(g3.is_node(v3));
	}
}

TEST_CASE("iterator constructor", "[gdwg.constructors]") {
	auto v1 = std::vector<int>{-1, 0, 1};
	auto v2 = std::set<std::string>{"a", "b", "c"};

	auto g1 = gdwg::graph<int, std::string>{v1.begin(), v1.end()};
	auto g2 = gdwg::graph<std::string, std::string>{v2.begin(), v2.end()};

	SECTION("type int") {
		CHECK(g1.is_node(-1));
		CHECK(g1.is_node(0));
		CHECK(g1.is_node(1));
	}

	SECTION("type string") {
		CHECK(g2.is_node("a"));
		CHECK(g2.is_node("b"));
		CHECK(g2.is_node("c"));
	}
}

TEST_CASE("copy constructor", "[gdwg.constructors]") {
	auto const g1 = gdwg::graph<std::string, int>{"Wang", "Liao", "Shi", "Fan"};
	auto copy_g1 = gdwg::graph(g1);

	// check if the copy is correct.
	CHECK(copy_g1.is_node("Wang"));
	CHECK(copy_g1.is_node("Liao"));
	CHECK(copy_g1.is_node("Shi"));
	CHECK(copy_g1.is_node("Fan"));

	// modify the copy and check if the original is not changed.
	copy_g1.insert_node("Chen");
	CHECK_FALSE(g1.is_node("Chen"));

	copy_g1.insert_edge("Wang", "Liao", 1);
	CHECK_FALSE(g1.is_connected("Wang", "Liao"));
}

TEST_CASE("move constructor", "[gdwg.constructors]") {
	auto const g1 = gdwg::graph<std::string, int>{"Wang", "Liao", "Shi", "Fan"};
	auto move_g1 = gdwg::graph(std::move(g1));

	// check if the move is correct.
	CHECK(move_g1.is_node("Wang"));
	CHECK(move_g1.is_node("Liao"));
	CHECK(move_g1.is_node("Shi"));
	CHECK(move_g1.is_node("Fan"));

	// modify the move and check if the original is not changed.
	move_g1.insert_node("Chen");
	CHECK_FALSE(g1.is_node("Chen"));

	move_g1.insert_edge("Wang", "Liao", 1);
	CHECK_FALSE(g1.is_connected("Wang", "Liao"));
}

TEST_CASE("copy assignment operator", "[gdwg.constructors]") {
	auto const g1 = gdwg::graph<std::string, int>{"Wang", "Liao", "Shi", "Fan"};
	auto copy_g1 = gdwg::graph<std::string, int>{"Wang", "Liao", "Shi", "Fan"};
	copy_g1 = g1;

	// check if the copy is correct.
	CHECK(copy_g1.is_node("Wang"));
	CHECK(copy_g1.is_node("Liao"));
	CHECK(copy_g1.is_node("Shi"));
	CHECK(copy_g1.is_node("Fan"));

	// modify the copy and check if the original is not changed.
	copy_g1.insert_node("Chen");
	CHECK_FALSE(g1.is_node("Chen"));

	copy_g1.insert_edge("Wang", "Liao", 1);
	CHECK_FALSE(g1.is_connected("Wang", "Liao"));
}

TEST_CASE("move assignment operator", "[gdwg.constructors]") {
	auto const g1 = gdwg::graph<std::string, int>{"Wang", "Liao", "Shi", "Fan"};
	auto move_g1 = gdwg::graph<std::string, int>{"Wang", "Liao", "Shi", "Fan"};
	move_g1 = std::move(g1);

	// check if the move is correct.
	CHECK(move_g1.is_node("Wang"));
	CHECK(move_g1.is_node("Liao"));
	CHECK(move_g1.is_node("Shi"));
	CHECK(move_g1.is_node("Fan"));

	// modify the move and check if the original is not changed.
	move_g1.insert_node("Chen");
	CHECK_FALSE(g1.is_node("Chen"));

	move_g1.insert_edge("Wang", "Liao", 1);
	CHECK_FALSE(g1.is_connected("Wang", "Liao"));
}