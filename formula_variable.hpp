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

			if (previous_branch.contains_literal(std::make_pair(!developed.truth_value_, symbol_))) {
				
				previous_branch.head_->childs_.push_back(std::make_unique<tableaux_tree_node>(nullptr, false));
				previous_branch.head_->childs_[0]->parent_ = previous_branch.head_;
				return branches;
			}
			else {
				previous_branch.add_literal(std::make_pair(developed.truth_value_, symbol_));
			}

			tableaux_tree_node* ptr_ = previous_branch.head_;

			if (previous_branch.has_formula_queue_empty()) {

				if (previous_branch.has_axiom_queue_empty()) {
					return branches;
				}

				previous_branch.head_->childs_.push_back(std::make_unique<tableaux_tree_node>(previous_branch.get_axiom_queue_top()));
			}
			else {
				previous_branch.head_->childs_.push_back(std::make_unique<tableaux_tree_node>(previous_branch.get_formula_queue_top()));
			}

			previous_branch.head_ = previous_branch.head_->childs_[previous_branch.head_->childs_.size() - 1].get();
			previous_branch.head_->parent_ = ptr_;
			branches.push_back(previous_branch);
			return branches;
		}
	private:
		char symbol_;
	};
}

#endif
