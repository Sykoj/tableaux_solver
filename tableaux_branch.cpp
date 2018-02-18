#include "tableaux_node.hpp"
#include "formula.hpp"
#include "tableaux_branch.hpp"

namespace tableaux {

	tableaux_branch::tableaux_branch(node_observer_ptr ptr) : head_(ptr) {}

	tableaux_branch::tableaux_branch(const tableaux_branch& tableaux_branch, const std::vector<tableaux_tree_node> new_nodes) {
		
		*this = tableaux_branch;
		for (const auto& node_ptr : new_nodes) {
			formula_queue_.push(node_ptr);
		}
	}

	std::vector<tableaux_branch> tableaux_branch::develop() {

		// gets item (formula) on top of the tableaux_branch
		auto formula_ptr = head_->formula_ptr_;
		// fork tableaux_branch by atomic tableax
		return formula_ptr->atomic_fork(*this, *head_);
	}
}
