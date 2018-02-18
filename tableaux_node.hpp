#ifndef TABLEAUX_NODE_HPP_
#define TABLEAUX_NODE_HPP_

#include <memory>
#include <vector>
#include <iostream>

namespace tableaux {

class formula;

class tableaux_tree_node {

	friend class formula_conjunction;
	friend class formula_disjunction;
	friend class formula_equivalence;
	friend class formula_implication;
	friend class formula_negation;
	friend class formula_variable;
	friend class tableaux_branch;
	friend class tableaux_printer;

	using formula_observer_ptr = const formula*;
	using tree_node_ptr = std::unique_ptr<tableaux_tree_node>;

  public:
	tableaux_tree_node(formula_observer_ptr formula_ptr, const bool truth_value) : 
		
		formula_ptr_(formula_ptr),
		truth_value_(truth_value),
		parent_(nullptr),
		subtree_horizontal_length_(0),
		print_start_index_(0),
		print_mark_index_(0) {}

	tableaux_tree_node &operator=(const tableaux_tree_node& node) {
		
		formula_ptr_ = node.formula_ptr_;
		truth_value_ = node.truth_value_;
		subtree_horizontal_length_ = node.subtree_horizontal_length_;
		parent_ = node.parent_;
		
		for (const auto& child : node.childs_)
    		childs_.push_back(std::make_unique<tableaux_tree_node>(*child));

		return *this;
	}

	tableaux_tree_node(const tableaux_tree_node &node) {

		formula_ptr_ = node.formula_ptr_;
		truth_value_ = node.truth_value_;
		subtree_horizontal_length_ = node.subtree_horizontal_length_;
		parent_ = node.parent_;

		for (const auto& child : node.childs_)
    		childs_.push_back(std::make_unique<tableaux_tree_node>(*child));
	}

	private:
		formula_observer_ptr formula_ptr_;
		bool truth_value_;
		tableaux_tree_node* parent_;
		std::vector<tree_node_ptr> childs_;

		size_t subtree_horizontal_length_;
		size_t print_start_index_;
		size_t print_mark_index_;
};
}

#endif
