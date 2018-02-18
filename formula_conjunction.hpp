#ifndef  FORMULA_CONJUNCTION_HPP_
#define  FORMULA_CONJUNCTION_HPP_
#include "formula_binary.hpp"

namespace tableaux {

	class formula_conjunction final : public formula_binary {

	public:
		formula_conjunction(formula_ptr&& rhs, formula_ptr&& lhs) {
			rhs_ = std::move(rhs);
			lhs_ = std::move(lhs);
		}

		const std::string& set_string_representation() override {

			string_representation_ = '(' + lhs_->set_string_representation() +" AND " + rhs_->set_string_representation() + ')';
			return string_representation_;
		}

		std::vector<tableaux_branch> atomic_fork(tableaux_branch& previous, const tableaux_tree_node& developed) const override {
			
			std::vector<tableaux_branch> new_branches;

			// T (lhs AND rhs) : 1 forked tableaux_branch
			if (developed.truth_value_ == true) {

				// T (rhs), T (lhs) forked tableaux_branch
				std::vector<tableaux_tree_node> new_nodes;
				new_nodes.emplace_back(rhs_.get(), true);
				new_nodes.emplace_back(lhs_.get(), true);
				new_branches.emplace_back(previous, new_nodes);
			}

			// F (lhs AND rhs) : 2 forked branches
			else {

				// F (rhs) new tableaux_branch
				std::vector<tableaux_tree_node> new_nodes;
				new_nodes.emplace_back(rhs_.get(), false);
				new_branches.emplace_back(previous, new_nodes);
				
				// F (lhs) new tableaux_branch
				new_nodes.clear();
				new_nodes.emplace_back(lhs_.get(), false);
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
