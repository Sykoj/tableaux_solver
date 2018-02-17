#include "item.hpp"
#include "formula.hpp"
#include "branch.hpp"

namespace tableaux {

	branch::branch(item_observer_ptr ptr) : head_(ptr) {}

	branch::branch(const branch& branch, const std::vector<item> new_items) {
		
		*this = branch;
		for (const auto& ptr : new_items) {
			formula_queue_.push(ptr);
		}
	}

	std::vector<branch> branch::develop() {

		// gets item (formula) on top of the branch
		auto formula_ptr = head_->formula_ptr;
		// fork branch by atomic tableax
		return formula_ptr->atomic_fork(*this, *head_);
	}
}
