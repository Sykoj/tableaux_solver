#ifndef  BRANCH_HPP_
#define  BRANCH_HPP_

#include <vector>
#include <queue>
#include <set>
#include <stack>
#include "item.hpp"

namespace tableaux {

	// prepositional logic literal
	using literal = std::pair<bool, char>;
	using item_observer_ptr = tableaux::item*;

	/*
	/  Observer structure for tableaux algorithm (queue-like, BFS).
	/  Stores important information for algorithm.
	/  Operates over tree (representing tableax) with ownership of nodes.
	*/
	class branch {

	public:
		branch() = default;
		// 'copy' constructor for algorithm runtime, adding new items into existing branch copy
		branch(const branch& branch, const std::vector<tableaux::item> new_items);
		// constructor for algorithm start
		branch(item_observer_ptr);

		// method for tableaux algorithm, providing forked branches into queue
		std::vector<branch> develop();
		
		friend class solver;

		void add_axiom(const tableaux::item& axiom) {
			axiom_queue_.push(axiom);
		}

		bool has_formula_queue_empty() const {
			return formula_queue_.empty();
		}

		bool has_axiom_queue_empty() const {
			return axiom_queue_.empty();
		}

		bool contains_literal(literal literal) {
			return (used_variables_.find(literal) != used_variables_.end());
		}

		void add_literal(literal literal) {
			used_variables_.insert(literal);
		}

		tableaux::item get_formula_queue_top() {
			auto ptr = formula_queue_.top();
			formula_queue_.pop();
			return ptr;
		}

		tableaux::item get_axiom_queue_top() {
			auto ptr = axiom_queue_.front();
			axiom_queue_.pop();
			return ptr;
		}

		item_observer_ptr head_;
	private:
		std::set<literal> used_variables_;
		/* 
		/	using stack instead of queue in current version is implementation detail
		/	explained in documentation
		*/
		std::stack<tableaux::item> formula_queue_;
		std::queue<tableaux::item> axiom_queue_;
	};
}

#endif
