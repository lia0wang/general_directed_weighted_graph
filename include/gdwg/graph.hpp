#ifndef GDWG_GRAPH_HPP
#define GDWG_GRAPH_HPP

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <stdexcept>
#include <tuple>
#include <utility>
#include <vector>

// This will not compile straight away
namespace gdwg {
	template<typename N, typename E>
	class graph {
	public:
		/* 2.2 Constructors */
		graph() noexcept = default;

		graph(std::initializer_list<N> il) {
			*this = graph(il.begin(), il.end());
		}

		template<typename InputIt>
		graph(InputIt first, InputIt last) {
			std::for_each(first, last, [this](auto const& n) { insert_node(n); });
		}

		graph(graph&& other) noexcept {
			*this = std::move(other);
		}

		auto operator=(graph&& other) noexcept -> graph& {
			if (this not_eq &other) {
				nodes_ = std::move(other.nodes_);
				edges_ = std::move(other.edges_);
			}
			return *this;
		}

		graph(graph const& other) {
			std::transform(other.nodes_.begin(),
			               other.nodes_.end(),
			               std::inserter(nodes_, nodes_.begin()),
			               [](auto const& n) { return std::make_shared<N>(*n); });
			std::transform(other.edges_.begin(),
			               other.edges_.end(),
			               std::inserter(edges_, edges_.begin()),
			               [](auto const& e) { return std::make_shared<edge_type>(*e); });
		}

		auto operator=(graph const& other) -> graph& {
			if (this not_eq &other) {
				std::transform(other.nodes_.begin(),
				               other.nodes_.end(),
				               std::inserter(nodes_, nodes_.begin()),
				               [](auto const& n) { return std::make_shared<N>(*n); });
				std::transform(other.edges_.begin(),
				               other.edges_.end(),
				               std::inserter(edges_, edges_.begin()),
				               [](auto const& e) { return std::make_shared<edge_type>(*e); });
			}
			return *this;
		}

		class iterator;

		/* 2.3 Modifiers */
		auto insert_node(N const& value) -> bool {
			auto const new_value = std::make_shared<N>(value);
			return nodes_.emplace(new_value).second;
		}

		auto insert_edge(N const& src, N const& dst, E const& weight) -> bool {
			auto const error_msg = std::string("Cannot call gdwg::graph<N, E>::insert_edge when")
			                          .append(" either src or dst node does not exist");
			if (not is_node(src) or not is_node(dst)) {
				throw std::runtime_error(error_msg);
			}

			auto const new_edge = std::make_shared<edge_type>();
			new_edge->from = nodes_.find(src)->get();
			new_edge->to = nodes_.find(dst)->get();
			new_edge->weight = weight;
			return edges_.emplace(new_edge).second;
		}

		auto replace_node(N const& old_data, N const& new_data) -> bool {
			auto const error_msg = std::string("Cannot call gdwg::graph<N, E>::replace_node on a")
			                          .append(" node that doesn't exist");
			if (not is_node(old_data)) {
				throw std::runtime_error(error_msg);
			}

			if (is_node(new_data)) {
				return false;
			}

			insert_node(new_data);
			merge_replace_node(old_data, new_data);
			return true;
		}

		auto merge_replace_node(N const& old_data, N const& new_data) -> void {
			auto const error_msg = std::string("Cannot call gdwg::graph<N, E>::merge_replace_node")
			                          .append(" on old or new data if they don't exist in the graph");
			if (not is_node(old_data) or not is_node(new_data)) {
				throw std::runtime_error(error_msg);
			}

			auto edges_to_replace = std::vector<std::shared_ptr<edge_type>>();
			std::copy_if(
			   edges_.begin(),
			   edges_.end(),
			   std::back_inserter(edges_to_replace),
			   [old_data](auto const& e) { return *e->from == old_data or *e->to == old_data; });

			std::for_each(edges_to_replace.begin(),
			              edges_to_replace.end(),
			              [this, old_data, new_data](auto const& e) {
				              auto const new_edge = std::make_shared<edge_type>();
				              new_edge->from = *e->from == old_data ? nodes_.find(new_data)->get()
				                                                    : e->from;
				              new_edge->to = *e->to == old_data ? nodes_.find(new_data)->get() : e->to;
				              new_edge->weight = e->weight;

				              edges_.emplace(new_edge);
				              edges_.erase(e);
			              });

			nodes_.erase(nodes_.find(old_data));
		}

		auto erase_node(N const& value) -> bool {
			if (not is_node(value)) {
				return false;
			}
			auto node_to_remove = nodes_.find(value);
			std::erase_if(edges_,
			              [value](auto const& e) { return *e->from == value or *e->to == value; });
			nodes_.erase(node_to_remove);

			return true;
		}

		/* Complexity: O(log (n) + e)
		 * is_node() has a complexity of O(log (n))
		 * std::find_if() has a complexity of O(e) since it iterates over the edges
		 */
		auto erase_edge(N const& src, N const& dst, E const& weight) -> bool {
			auto const error_msg = std::string("Cannot call gdwg::graph<N, E>::erase_edge on")
			                          .append(" src or dst if they don't exist in the graph");
			if (not is_node(src) or not is_node(dst)) {
				throw std::runtime_error(error_msg);
			}

			auto const edge_to_remove =
			   std::find_if(edges_.begin(), edges_.end(), [src, dst, weight](auto const& e) {
				   return std::tie(*e->from, *e->to, e->weight) == std::tie(src, dst, weight);
			   });
			if (edge_to_remove == edges_.end()) {
				return false;
			}
			edges_.erase(edge_to_remove);
			return true;
		}

		/* Complexity: Amortised constant time.
		 */
		auto erase_edge(iterator i) -> iterator {
			return i == end() or i == iterator{} ? end() : iterator(edges_.erase(i.e_it_));
		}

		/* Complexity O(d)
		 */
		auto erase_edge(iterator i, iterator s) -> iterator {
			return i == end() or i == iterator{} ? end() : iterator(edges_.erase(i.e_it_, s.e_it_));
		}

		auto clear() noexcept -> void {
			nodes_.clear();
			edges_.clear();
		}

		/* 2.4 Accessors */

		/* Complexity: O(log(n))
		 * std::set::find has O(log(n)) complexity to search for a value in a set
		 */
		[[nodiscard]] auto is_node(N const& value) const -> bool {
			return nodes_.find(value) not_eq nodes_.end();
		}

		[[nodiscard]] auto empty() const -> bool {
			return nodes_.empty();
		}

		[[nodiscard]] auto is_connected(N const& src, N const& dst) const -> bool {
			auto const error_msg = std::string("Cannot call gdwg::graph<N, E>::is_connected if src")
			                          .append(" or dst node don't exist in the graph");
			if (not is_node(src) or not is_node(dst)) {
				throw std::runtime_error(error_msg);
			}

			return std::any_of(edges_.begin(), edges_.end(), [&](auto const& e) {
				return std::tie(*e->from, *e->to) == std::tie(src, dst);
			});
		}

		/* Complexity: O(n)
		 * std::transform has O(n) complexity based on the distance between the begin and end
		 */
		[[nodiscard]] auto nodes() const -> std::vector<N> {
			auto nodes = std::vector<N>{};
			std::transform(nodes_.begin(), nodes_.end(), std::back_inserter(nodes), [](auto const& n) {
				return *n;
			});
			return nodes;
		}

		/* Complexity: O(log (n) + e)
		 * is_node() has O(log(n)) complexity by using std::set::find
		 * std::for_each has O(e) complexity since it iterates over all the edges
		 */
		[[nodiscard]] auto weights(N const& src, N const& dst) const -> std::vector<E> {
			auto const error_msg = std::string("Cannot call gdwg::graph<N, E>::weights if src")
			                          .append(" or dst node don't exist in the graph");
			if (not is_node(src) or not is_node(dst)) {
				throw std::runtime_error(error_msg);
			}
			auto weights = std::vector<E>{};
			std::for_each(edges_.begin(), edges_.end(), [&](auto const& e) {
				if (std::tie(*e->from, *e->to) == std::tie(src, dst)) {
					weights.push_back(e->weight);
				}
			});
			return weights;
		}

		/* Complexity:  O(log (n) + log (e))
		 * is_node() has O(log(n)) complexity by using std::set::find
		 * std::set::find has O(log(e)) complexity to search for an edge in a set
		 */
		[[nodiscard]] auto find(N const& src, N const& dst, E const& weight) const -> iterator {
			if (not is_node(src) or not is_node(dst)) {
				return end();
			}
			return iterator(edges_.find(value_type{src, dst, weight}));
		}

		/* Complexity:  O(log (n) + e)
		 * is_node() has O(log(n)) complexity by using std::set::find
		 * std::for_each has O(e) complexity since it iterates over all the edges
		 */
		[[nodiscard]] auto connections(N const& src) const -> std::vector<N> {
			auto const error_msg = std::string("Cannot call gdwg::graph<N, E>::connections if src")
			                          .append(" doesn't exist in the graph");
			if (not is_node(src)) {
				throw std::runtime_error(error_msg);
			}
			auto connections = std::vector<N>{};
			std::for_each(edges_.begin(), edges_.end(), [&](auto const& e) {
				if (*e->from == src) {
					connections.push_back(*e->to);
				}
			});
			return connections;
		}

		/* 2.5 Iterator access */
		[[nodiscard]] auto begin() const -> iterator {
			return iterator(edges_.begin());
		}
		[[nodiscard]] auto end() const -> iterator {
			return iterator(edges_.end());
		}

		/* 2.6 Comparisons */
		/* Complexity: O(n + e)
		 * std::all_of has O(n) complexity to iterate over all the nodes
		 * std::all_of has O(e) complexity to iterate over all the edges
		 */
		[[nodiscard]] auto operator==(graph const& other) const -> bool {
			if (nodes_.size() not_eq other.nodes_.size() or edges_.size() not_eq other.edges_.size()) {
				return false;
			}
			return std::all_of(nodes_.begin(),
			                   nodes_.end(),
			                   [&](auto const& n) { return other.is_node(*n); })
			       and std::all_of(edges_.begin(), edges_.end(), [&](auto const& e) {
				           return other.find(*e->from, *e->to, e->weight) not_eq other.end();
			           });
		}

		/* 2.7 Extractor*/
		friend auto operator<<(std::ostream& os, graph const& g) -> std::ostream& {
			std::for_each(g.nodes_.begin(), g.nodes_.end(), [&](auto const& src) {
				os << *src << " (\n";
				auto const connections = g.connections(*src);
				std::for_each(connections.begin(), connections.end(), [&](auto const& dst) {
					auto const weights = g.weights(*src, dst);
					std::for_each(weights.begin(), weights.end(), [&](auto const& weight) {
						os << "  " << dst << " | " << weight << "\n";
					});
				});
				os << ")\n";
			});
			return os;
		}

		struct value_type {
			N from;
			N to;
			E weight;
		};

	private:
		struct edge_type {
			N* from;
			N* to;
			E weight;
		};

		struct node_compare {
			using is_transparent = void;
			auto operator()(std::shared_ptr<N> const& first, N const& second) const -> bool {
				assert(first not_eq nullptr);
				return *first < second;
			}
			auto operator()(N const& first, std::shared_ptr<N> const& second) const -> bool {
				assert(second not_eq nullptr);
				return first < *second;
			}
			auto operator()(std::shared_ptr<N> const& first, std::shared_ptr<N> const& second) const
			   -> bool {
				assert(std::tie(first, second) not_eq std::make_tuple(nullptr, nullptr));
				return *first < *second;
			}
		};

		struct edge_compare {
			using is_transparent = void;
			auto operator()(std::shared_ptr<edge_type> const& first,
			                std::shared_ptr<edge_type> const& second) const -> bool {
				assert(std::tie(first, second) not_eq std::make_tuple(nullptr, nullptr));
				return std::tie(*first->from, *first->to, first->weight)
				       < std::tie(*second->from, *second->to, second->weight);
			}
			auto operator()(std::shared_ptr<edge_type> const& first, edge_type const& second) const
			   -> bool {
				assert(first not_eq nullptr);
				return std::tie(*first->from, *first->to, first->weight)
				       < std::tie(*second.from, *second.to, second.weight);
			}
			auto operator()(edge_type const& first, std::shared_ptr<edge_type> const& second) const
			   -> bool {
				assert(second not_eq nullptr);
				return std::tie(*first.from, *first.to, first.weight)
				       < std::tie(*second->from, *second->to, second->weight);
			}
			auto operator()(std::shared_ptr<edge_type> const& first, value_type const& second) const
			   -> bool {
				assert(first not_eq nullptr);
				return std::tie(*first->from, *first->to, first->weight)
				       < std::tie(second.from, second.to, second.weight);
			}
			auto operator()(value_type const& first, std::shared_ptr<edge_type> const& second) const
			   -> bool {
				assert(second not_eq nullptr);
				return std::tie(first.from, first.to, first.weight)
				       < std::tie(*second->from, *second->to, second->weight);
			}
		};

		std::set<std::shared_ptr<N>, node_compare> nodes_;
		std::set<std::shared_ptr<edge_type>, edge_compare> edges_;

	public:
		/* 2.8 Iterator */
		class iterator {
			using edge_it = typename std::set<std::shared_ptr<edge_type>, edge_compare>::iterator;

		public:
			using value_type = graph<N, E>::value_type;
			using reference = value_type;
			using pointer = void;
			using difference_type = std::ptrdiff_t;
			using iterator_category = std::bidirectional_iterator_tag;

			iterator() = default;

			auto operator*() const -> reference {
				auto return_val = value_type{};
				return_val.from = *((*e_it_)->from);
				return_val.to = *((*e_it_)->to);
				return_val.weight = (*e_it_)->weight;
				return return_val;
			}

			auto operator++() -> iterator& {
				++e_it_;
				return *this;
			}
			auto operator++(int) -> iterator {
				auto temp = *this;
				++*this;
				return temp;
			}
			auto operator--() -> iterator& {
				--e_it_;
				return *this;
			}
			auto operator--(int) -> iterator {
				auto temp = *this;
				--*this;
				return temp;
			}

			auto operator==(iterator const& other) const -> bool {
				return e_it_ == other.e_it_;
			}

		private:
			edge_it e_it_;
			friend class graph;
			explicit iterator(edge_it e_it)
			: e_it_(e_it) {}
		};
	};

} // namespace gdwg

#endif // GDWG_GRAPH_HPP
