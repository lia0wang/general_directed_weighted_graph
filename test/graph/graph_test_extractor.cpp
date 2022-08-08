/* @author: Wang Liao
 * @date: 2022-7
 * @rational: Mainly use gdwg.constructors & gdwg.modifiers & gdwg.accessors
              to test extractor.
 * @approach:
    * 1. Test extractor with empty graph.
    * 2. Test extractor with simple graph.
    * 3. Test extractor with complex graph.
 */

#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>

TEST_CASE("extractor with empty graph", "[gdwg.extractor]") {
	auto g = gdwg::graph<std::string, int>{};
	auto out = std::ostringstream{};
	out << g;
	CHECK(out.str() == "");
}

TEST_CASE("extractor with simple graph", "[gdwg.extractor]") {
	auto g = gdwg::graph<std::string, int>{};
	g.insert_node("wang");
	g.insert_node("liao");
	g.insert_edge("wang", "liao", 1);
	auto out = std::ostringstream{};
	out << g;
	auto const expected = std::string_view(R"(liao (
)
wang (
  liao | 1
)
)");
	CHECK(out.str() == expected);
}

TEST_CASE("extractor with complex graph", "[gdwg.extractor]") {
	auto g = gdwg::graph<int, int>();
	// (1, 7, 4),
	// (1, 12, 3),
	// (1, 21, 12),
	// (7, 21, 13),
	// (12, 19, 16),
	// (14, 14, 0),
	// (19, 1, 3),
	// (19, 21, 2),
	// (21, 14, 23),
	// (21, 31, 14),
	g.insert_node(1);
	g.insert_node(7);
	g.insert_node(12);
	g.insert_node(21);
	g.insert_node(14);
	g.insert_node(19);
	g.insert_node(31);
	g.insert_edge(1, 7, 4);
	g.insert_edge(1, 12, 3);
	g.insert_edge(1, 21, 12);
	g.insert_edge(7, 21, 13);
	g.insert_edge(12, 19, 16);
	g.insert_edge(14, 14, 0);
	g.insert_edge(19, 1, 3);
	g.insert_edge(19, 21, 2);
	g.insert_edge(21, 14, 23);
	g.insert_edge(21, 31, 14);
	auto out = std::ostringstream{};
	out << g;
	auto const expected = std::string_view(R"(1 (
  7 | 4
  12 | 3
  21 | 12
)
7 (
  21 | 13
)
12 (
  19 | 16
)
14 (
  14 | 0
)
19 (
  1 | 3
  21 | 2
)
21 (
  14 | 23
  31 | 14
)
31 (
)
)");
	CHECK(out.str() == expected);
}