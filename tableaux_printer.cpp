#include "tableaux_printer.hpp"
#include <ostream>

namespace tableaux {

	void tableaux_printer::print_tableaux(tableaux_tree_node* root, std::ostream& output) {

		calculate_bounds(root);
		calculate_formula_offsets(root, 0, root->print_length_);
		calculate_mark_offsets(root);

		std::vector<tableaux_tree_node*> bfs_queue;
		bfs_queue.push_back(root);
		std::string line_formulas(root->print_length_, ' ');
		std::string line_marks(root->print_length_, ' ');
		std::string line_connections(root->print_length_, ' ');

		while (!bfs_queue.empty()) {

			std::vector<tableaux_tree_node*> new_items;

			for (const auto& tableaux_tree_node : bfs_queue) {

				if (tableaux_tree_node->formula_ptr_ != nullptr) {

					std::string formula;
					if (tableaux_tree_node->formula_ptr_->string_representation_.length() == 1) {
						formula = (tableaux_tree_node->truth_value_) ? "T(" + tableaux_tree_node->formula_ptr_->string_representation_ + ')' :
							"F(" + tableaux_tree_node->formula_ptr_->string_representation_ + ')';
					}
					else {
						formula = (tableaux_tree_node->truth_value_) ? 'T' + tableaux_tree_node->formula_ptr_->string_representation_ :
							'F' + tableaux_tree_node->formula_ptr_->string_representation_;
					}

					for (int i = 0; i < formula.length(); i++) {

						line_formulas[tableaux_tree_node->print_start_index_ + i] = formula[i];
					}

					if (tableaux_tree_node->childs_.size() != 0) {
						line_marks[tableaux_tree_node->print_mark_index_] = '|';

						if (tableaux_tree_node->childs_.size() == 2)
							for (int i = tableaux_tree_node->childs_[0]->print_mark_index_; i <= tableaux_tree_node->childs_[1]->print_mark_index_; i++) {
								line_connections[i] = '-';
							}
						else line_connections[tableaux_tree_node->print_mark_index_] = '|';

					}
				}
				else {
					line_formulas[tableaux_tree_node->parent_->print_mark_index_] = 'X';
					//line_marks[tableaux_tree_node->print_start_index] = '|';
				}

				for (int i = 0; i < tableaux_tree_node->childs_.size(); i++) {

					new_items.push_back(tableaux_tree_node->childs_[i].get());
				}

			}

			output << line_formulas.c_str() << std::endl << line_marks.c_str() << std::endl <<
					  line_connections.c_str() << std::endl;
			//std::cout << line_marks;
			line_formulas = line_marks = line_connections = std::string(root->print_length_, ' '); // fix
			bfs_queue = new_items; 
		}
	}

	// Recursive function
	size_t tableaux_printer::calculate_bounds(tableaux_tree_node* it) {

		// Has no childs_ --> end of branch & is variable or contradiction mark
		if (it->childs_.size() == 0) {

			it->print_length_ = 4;
			return 4;
		}
		// Length of subtree, will be compared with length of current formula
		size_t subtree_print_length = 0;

		for (int i = 0; i < it->childs_.size(); i++) {

			// Child's formula is nullptr -> contradiction
			if (it->childs_[i]->formula_ptr_ == nullptr) {

				it->childs_[i]->print_length_ = 1;
				subtree_print_length += it->childs_[i]->print_length_;
			}
			// Else take size of subtree under child tableaux_tree_node
			else subtree_print_length += calculate_bounds(it->childs_[i].get());
		}

		// For each gap between childs_ add 10 characters
		subtree_print_length += 10 * (it->childs_.size() - 1);

		// Size of formula with tableaux wrapper
		size_t tableaux_formula_length = 0;
		if (it->formula_ptr_->string_representation_.length() == 1) // variable
			tableaux_formula_length = 4;  // T/F + parenthesis
		else
			tableaux_formula_length = it->formula_ptr_->string_representation_.length() + 1; // just T/F flag, parentheses already included

		it->print_length_ = tableaux_formula_length > subtree_print_length ?
			tableaux_formula_length : subtree_print_length;

		return it->print_length_;
	}

	void tableaux::tableaux_printer::calculate_formula_offsets(tableaux_tree_node* it, size_t start, size_t end) {

		size_t offset = 0;
		if (it->formula_ptr_ != nullptr) {
			
			if (it->formula_ptr_->string_representation_.length() == 1)
				offset = ((end - start) - (it->formula_ptr_->string_representation_.length() + 3)) / 2;
			else {
				offset = ((end - start) - (it->formula_ptr_->string_representation_.length() + 1)) / 2;
			}
		}
		else { // contradiction
			it->print_start_index_ = start;
			return;
		}

		// handling childs_

		it->print_start_index_ = start + offset;

		// get total print count of subtree
		size_t subtree_count = 0;
		for (int i = 0; i < it->childs_.size(); i++) {

			subtree_count += it->childs_[i]->print_length_;
		}
		subtree_count += 10 * (it->childs_.size() - 1);

		// get start offset for printing subtree items
		offset = ((end - start) - subtree_count) / 2;

		for (int i = 0; i < it->childs_.size(); i++) {

			calculate_formula_offsets(it->childs_[i].get(), offset + start, offset + start + it->childs_[i]->print_length_);
			offset += 10 + it->childs_[i]->print_length_;
		}
	}

	void tableaux::tableaux_printer::calculate_mark_offsets(tableaux_tree_node* it) {
	
		if (it->formula_ptr_ == nullptr) return;
		
		if (it->print_mark_index_ == 0) {
			
			if (it->print_length_ == 1)
				it->print_mark_index_ = it->print_start_index_ + 1;
			else
				it->print_mark_index_ = it->print_start_index_ + (it->formula_ptr_->string_representation_.length() + 1) / 2;
		}
	
		if (it->childs_.size() == 1) {

			it->childs_[0]->print_mark_index_ = it->print_mark_index_;
		}
	
		for (int i = 0; i < it->childs_.size(); i++) {
			calculate_mark_offsets(it->childs_[i].get());
		}
	}
}
