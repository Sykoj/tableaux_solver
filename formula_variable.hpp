#ifndef  FORMULA_VARIABLE_HPP_
#define  FORMULA_VARIABLE_HPP_
#include "formula.hpp"

namespace tableaux {

	class formula_variable final: public formula {
	
	public:
	
		formula_variable(const char symbol) : symbol_(symbol) {
		}

		const std::string& set_string_representation() override {
			
			string_representation_ = symbol_;
			return string_representation_;
		}

		std::vector<tableaux_branch> atomic_fork(tableaux_branch& previous_branch, const tableaux_tree_node& developed) const override  {

			std::vector<tableaux_branch> branches;

			// check for contradiction
			if (previous_branch.contains_literal(std::make_pair(!developed.truth_value_, symbol_))) {
				
				// add contradiction tableaux tree node <-> formula_ptr_ is nullptr
				previous_branch.head_->childs_.push_back(std::make_unique<tableaux_tree_node>(nullptr, false));
				previous_branch.head_->childs_[0]->parent_ = previous_branch.head_;
				return branches;
			}
			else {
				// add current variable literal with truth value to used variables
				previous_branch.add_literal(std::make_pair(developed.truth_value_, symbol_));
			}

			tableaux_tree_node* ptr_ = previous_branch.head_;

			if (previous_branch.has_formula_queue_empty()) {

				if (previous_branch.has_axiom_queue_empty()) {
					// no nodes can be developed <-> fully developed branch
					return branches;
				}

				previous_branch.head_->childs_.push_back(std::make_unique<tableaux_tree_node>(previous_branch.get_axiom_queue_top()));
			}
			else {
				previous_branch.head_->childs_.push_back(std::make_unique<tableaux_tree_node>(previous_branch.get_formula_queue_top()));
			}

			// add 'poped' tree node as child of parent node
			previous_branch.head_ = previous_branch.head_->childs_[previous_branch.head_->childs_.size() - 1].get();
			previous_branch.head_->parent_ = ptr_;
			// in case of variable as node, only one 'forked' branch exists
			branches.push_back(previous_branch);
			return branches;
		}
	private:
		char symbol_;
	};
}

#endif
