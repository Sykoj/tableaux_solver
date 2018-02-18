#ifndef  FORMULA_IMPLICATION_HPP_
#define  FORMULA_IMPLICATION_HPP_
#include "formula_binary.hpp"

namespace tableaux {

	class formula_implication final : public formula_binary {

	public:
		formula_implication(formula_ptr&& rhs, formula_ptr&& lhs) {
			rhs_ = std::move(rhs);
			lhs_ = std::move(lhs);
		}

		const std::string& set_string_representation() override {

			string_representation_ = '(' + lhs_->set_string_representation() + " -> " + rhs_->set_string_representation() + ')';
			return string_representation_;
		}

		std::vector<tableaux_branch> atomic_fork(tableaux_branch& previous, const tableaux_tree_node& developed) const override {

			std::vector<tableaux_branch> new_branches;

			// T (lhs IMP rhs) : 2 forked branches
			if (developed.truth_value_ == true) {
				
				// F (lhs) forked tableaux_branch
				std::vector<tableaux_tree_node> new_items;
				new_items.emplace_back(lhs_.get(), false);
				new_branches.emplace_back(previous, new_items);

				// T (rhs) forked tableaux_branch
				new_items.clear();
				new_items.emplace_back(rhs_.get(), true);
				new_branches.emplace_back(previous, new_items);
			}
			// F (lhs IMP rhs) : 1 forked tableaux_branch
			else {

				// T (lhs), F(rhs) new tableaux_branch
				std::vector<tableaux_tree_node> new_items;
				new_items.emplace_back(lhs_.get(), true);
				new_items.emplace_back(rhs_.get(), false);
				new_branches.emplace_back(previous, new_items);
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
