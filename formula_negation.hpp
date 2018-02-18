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

			string_representation_ = "(NOT " + subformula_->set_string_representation() + ')';
			return string_representation_;
		}
	
		std::vector<tableaux_branch> atomic_fork(tableaux_branch& previous, const tableaux_tree_node& developed) const override {

			std::vector<tableaux_branch> new_branches;

			// T (NOT subformula) : 1 forked tableaux_branch
			if (developed.truth_value_ == true) {

				// F (subformula) new tableaux_branch
				std::vector<tableaux_tree_node> new_nodes;
				new_nodes.emplace_back(subformula_.get(), false);
				new_branches.emplace_back(previous, new_nodes);

			}
			// F (NOT subformula) : 1 forked tableaux_branch
			else {
				
				// T (subformula) new tableaux_branch
				std::vector<tableaux_tree_node> new_nodes;
				new_nodes.emplace_back(subformula_.get(), true);
				new_branches.emplace_back(previous, new_nodes);
			}

			// foreach new tableaux_branch move head forwards
			for (auto& branch : new_branches) {

				// get observer pointer to head (tree node)
				tableaux_tree_node* ptr = branch.head_;
				// add new child to node (node is owner)
				branch.head_->childs_.push_back(std::make_unique<tableaux_tree_node>(branch.get_formula_queue_top()));
				// move head of tableaux_branch to this child
				branch.head_ = branch.head_->childs_[branch.head_->childs_.size() - 1].get();
				// set previous head as child's parent
				branch.head_->parent_ = ptr;
			}

			return new_branches;
		}
	};
}

#endif
