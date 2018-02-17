#ifndef  FORMULA_VARIABLE_HPP_
#define  FORMULA_VARIABLE_HPP_
#include "formula.hpp"

namespace tableaux {

	class formula_variable final: public formula {
	
	public:
	
		formula_variable(const char symbol) : symbol_(symbol) {
		}

		const std::string& set_string_representation() override {
			
			string_representation = symbol_;
			return string_representation;
		}

		std::vector<branch> atomic_fork(branch& previous_branch, const item& developed) const override  {

			std::vector<branch> branches;

			if (previous_branch.contains_literal(std::make_pair(!developed.truth_value, symbol_))) {
				
				previous_branch.head_->childs.push_back(std::make_unique<item>(nullptr, false));
				previous_branch.head_->childs[0]->parent = previous_branch.head_;
				return branches;
			}
			else {
				previous_branch.add_literal(std::make_pair(developed.truth_value, symbol_));
			}

			item* ptr_ = previous_branch.head_;

			if (previous_branch.has_formula_queue_empty()) {

				if (previous_branch.has_axiom_queue_empty()) {
					return branches;
				}

				previous_branch.head_->childs.push_back(std::make_unique<item>(previous_branch.get_axiom_queue_top()));
			}
			else {
				previous_branch.head_->childs.push_back(std::make_unique<item>(previous_branch.get_formula_queue_top()));
			}

			previous_branch.head_ = previous_branch.head_->childs[previous_branch.head_->childs.size() - 1].get();
			previous_branch.head_->parent = ptr_;
			branches.push_back(previous_branch);
			return branches;
		}
	private:
		char symbol_;
	};
}

#endif
