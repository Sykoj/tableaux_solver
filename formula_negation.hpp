#ifndef  FORMULA_NEGATION_HPP_
#define  FORMULA_NEGATION_HPP_
#include "formula_unary.hpp"

namespace tableaux {

	class formula_negation final: public formula_unary {
	
	public:
		formula_negation(formula_ptr&& subformula) {
			subformula_ = std::move(subformula);
		}

		const std::string& set_string_representation() override {

			string_representation = "(NOT " + subformula_->set_string_representation() + ')';
			return string_representation;
		}
	
		std::vector<branch> atomic_fork(branch& previous, const item& developed) const override {

			std::vector<branch> new_branches;

			// T (NOT subformula) : 1 forked branch
			if (developed.truth_value == true) {

				// F (subformula) new branch
				std::vector<item> new_items;
				new_items.emplace_back(subformula_.get(), false);
				new_branches.emplace_back(previous, new_items);

			}
			// F (NOT subformula) : 1 forked branch
			else {
				
				// T (subformula) new branch
				std::vector<item> new_items;
				new_items.emplace_back(subformula_.get(), true);
				new_branches.emplace_back(previous, new_items);
			}

			// foreach new branch move head forwards
			for (auto& branch : new_branches) {

				// get observer pointer to head (tree node)
				item* ptr = branch.head_;
				// add new child to node (node is owner)
				branch.head_->childs.push_back(std::make_unique<item>(branch.get_formula_queue_top()));
				// move head of branch to this child
				branch.head_ = branch.head_->childs[branch.head_->childs.size() - 1].get();
				// set previous head as child's parent
				branch.head_->parent = ptr;
			}

			return new_branches;
		}
	};
}

#endif
