#ifndef  TABLEAUX_BRANCH_HPP_
#define  TABLEAUX_BRANCH_HPP_

#include <vector>
#include <queue>
#include <set>
#include <stack>
#include "tableaux_node.hpp"

namespace tableaux {

	// prepositional logic literal
	using literal = std::pair<bool, char>;
	using node_observer_ptr = tableaux_tree_node*;

	class tableaux_branch {

		friend class solver;
		friend class formula_conjunction;
		friend class formula_disjunction;
		friend class formula_equivalence;
		friend class formula_implication;
		friend class formula_negation;
		friend class formula_variable;

	public:
		tableaux_branch() = default;
		// 'copy' constructor for algorithm runtime, adding new items into existing tableaux_branch copy
		tableaux_branch(const tableaux_branch& tableaux_branch, const std::vector<tableaux_tree_node> new_nodes);
		// constructor for algorithm start
		tableaux_branch(node_observer_ptr);

		// method for tableaux algorithm, providing forked branches into queue
		std::vector<tableaux_branch> develop();

		void add_axiom(const tableaux_tree_node& axiom) {
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

		tableaux_tree_node get_formula_queue_top() {
			auto node_ptr = formula_queue_.top();
			formula_queue_.pop();
			return node_ptr;
		}

		tableaux_tree_node get_axiom_queue_top() {
			auto node_ptr = axiom_queue_.front();
			axiom_queue_.pop();
			return node_ptr;
		}

	private:
		node_observer_ptr head_;
		std::set<literal> used_variables_;
		/* 
		/	using stack instead of queue in current version is implementation detail
		/	explained in documentation.
		*/
		std::stack<tableaux::tableaux_tree_node> formula_queue_;
		std::queue<tableaux::tableaux_tree_node> axiom_queue_;
	};
}

#endif
