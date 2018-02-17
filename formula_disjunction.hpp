#ifndef  FORMULA_DISJUNCTION_HPP_
#define  FORMULA_DISJUNCTION_HPP_
#include "formula_binary.hpp"


namespace tableaux {

	class formula_disjunction final: public formula_binary {

	public:
		formula_disjunction(formula_ptr&& rhs, formula_ptr&& lhs) {
			rhs_ = std::move(rhs);
			lhs_ = std::move(lhs);
		}

		const std::string& set_string_representation() override {

			string_representation = '(' + lhs_->set_string_representation() + " OR " + rhs_->set_string_representation() + ')';
			return string_representation;
		}

		std::vector<branch> atomic_fork(branch& previous, const item& developed) const override {

			std::vector<branch> new_branches;

			// T (lhs AND rhs) : 2 forked branches
			if (developed.truth_value == true) {

				// T (lhs) new branch
				std::vector<item> new_items;
				new_items.emplace_back(lhs_.get(), true);
				new_branches.emplace_back(previous, new_items);

				// T (rhs) new branch
				new_items.clear();
				new_items.emplace_back(rhs_.get(), true);
				new_branches.emplace_back(previous, new_items);
			}

			// F (lhs OR rhs) : 1 forked branch
			else {

				// F (lhs), F (rhs) forked branch
				std::vector<item> new_items;
				new_items.emplace_back(lhs_.get(), false);
				new_items.emplace_back(rhs_.get(), false);
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
