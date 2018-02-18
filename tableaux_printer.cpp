#include "tableaux_printer.hpp"
#include <ostream>

namespace tableaux {

	void tableaux_printer::print_tableaux(tableaux_tree_node* root, std::ostream& output) {

		calculate_subtree_block_length(root);
		calculate_formula_line_offsets(root, 0, root->subtree_horizontal_length_);
		calculate_mark_line_offsets(root);

		std::vector<tableaux_tree_node*> bfs_queue;
		bfs_queue.push_back(root);

		// line for formula printing
		std::string line_formulas(root->subtree_horizontal_length_, ' ');
		// line for printing marks '|' under formulas
		std::string line_marks(root->subtree_horizontal_length_, ' ');
		// line for printing connections "---------" between two nodes or marks '|'
		std::string line_connections(root->subtree_horizontal_length_, ' ');

		while (!bfs_queue.empty()) {

			std::vector<tableaux_tree_node*> new_items;

			for (const auto& tableaux_tree_node : bfs_queue) {

				// node is not contradiction
				if (tableaux_tree_node->formula_ptr_ != nullptr) {

					std::string formula;
					// variable
					if (tableaux_tree_node->formula_ptr_->string_representation_.length() == 1) {

						formula = (tableaux_tree_node->truth_value_) ? "T(" + tableaux_tree_node->formula_ptr_->string_representation_ + ')' :
							"F(" + tableaux_tree_node->formula_ptr_->string_representation_ + ')';
					} // formula
					else {

						formula = (tableaux_tree_node->truth_value_) ? 'T' + tableaux_tree_node->formula_ptr_->string_representation_ :
							'F' + tableaux_tree_node->formula_ptr_->string_representation_;
					}

					// copy formula characters into line for printing formulas
					for (int i = 0; i < formula.length(); i++) {

						line_formulas[tableaux_tree_node->print_start_index_ + i] = formula[i];
					}

					// create marks between nodes
					if (tableaux_tree_node->childs_.size() != 0) {
						line_marks[tableaux_tree_node->print_mark_index_] = '|';

						// create node connections
						if (tableaux_tree_node->childs_.size() == 2)

							for (int i = tableaux_tree_node->childs_[0]->print_mark_index_; i <= tableaux_tree_node->childs_[1]->print_mark_index_; i++) {
								line_connections[i] = '-';
							}
						// no node connection -> lengthen mark instead
						else line_connections[tableaux_tree_node->print_mark_index_] = '|';

					}
				}
				else { // contradiction
					line_formulas[tableaux_tree_node->parent_->print_mark_index_] = 'X';
				}

				// continue BFS with child nodes
				for (int i = 0; i < tableaux_tree_node->childs_.size(); i++) {

					new_items.push_back(tableaux_tree_node->childs_[i].get());
				}

			}

			// print lines
			output << line_formulas.c_str() << std::endl << line_marks.c_str() << std::endl <<
					  line_connections.c_str() << std::endl;

			// reset lines for next printing
			line_formulas = line_marks = line_connections = std::string(root->subtree_horizontal_length_, ' ');
			bfs_queue = new_items; 
		}
	}

	size_t tableaux_printer::calculate_subtree_block_length(tableaux_tree_node* it) {

		// Has no childs_ --> end of branch & node is variable
		if (it->childs_.size() == 0) {

			it->subtree_horizontal_length_ = 4; // T(s), F(s) length where s is varible symbol
			return 4;
		}
		// Length of subtree, will be compared with length of current formula
		size_t subtree_print_length = 0;

		for (int i = 0; i < it->childs_.size(); i++) {

			// Child's formula is nullptr -> contradiction
			if (it->childs_[i]->formula_ptr_ == nullptr) {

				it->childs_[i]->subtree_horizontal_length_ = 1;
				subtree_print_length += it->childs_[i]->subtree_horizontal_length_;
			}
			// Else take size of subtree under child tableaux_tree_node
			else subtree_print_length += calculate_subtree_block_length(it->childs_[i].get());
		}

		// Add total length of gaps
		subtree_print_length += gap_ * (it->childs_.size() - 1);

		// Size of formula with tableaux wrapper
		size_t tableaux_formula_length = 0;
		if (it->formula_ptr_->string_representation_.length() == 1) // variable
			tableaux_formula_length = 4;  // T/F + parenthesis
		else
			tableaux_formula_length = it->formula_ptr_->string_representation_.length() + 1; // just T/F flag, parentheses already included

		it->subtree_horizontal_length_ = tableaux_formula_length > subtree_print_length ?
			tableaux_formula_length : subtree_print_length;

		return it->subtree_horizontal_length_;
	}

	void tableaux::tableaux_printer::calculate_formula_line_offsets(tableaux_tree_node* it, size_t start, size_t end) {

		size_t offset = 0;

		// align formula to the centre of allowed range (start, end)
		if (it->formula_ptr_ != nullptr) {
			
			if (it->formula_ptr_->string_representation_.length() == 1) // variable +3 T() or F() characters
				offset = ((end - start) - (it->formula_ptr_->string_representation_.length() + 3)) / 2;
			else { // formula +1 T or F additional character
				offset = ((end - start) - (it->formula_ptr_->string_representation_.length() + 1)) / 2;
			}
		}
		else { // contradiction
			it->print_start_index_ = start;
			return;
		}

		// handling childs

		it->print_start_index_ = start + offset;

		// get total print length of subtree
		size_t subtree_count = 0;
		for (int i = 0; i < it->childs_.size(); i++) {

			subtree_count += it->childs_[i]->subtree_horizontal_length_;
		}
		subtree_count += gap_ * (it->childs_.size() - 1);	

		// align subtree to the centre of allowed range
		offset = ((end - start) - subtree_count) / 2;

		for (int i = 0; i < it->childs_.size(); i++) {

			// for each child node, set allowed range and continue recursion
			calculate_formula_line_offsets(it->childs_[i].get(), offset + start, offset + start + it->childs_[i]->subtree_horizontal_length_);
			offset += gap_ + it->childs_[i]->subtree_horizontal_length_;
		}
	}

	void tableaux::tableaux_printer::calculate_mark_line_offsets(tableaux_tree_node* it) {
		
		// contradiction -> no marks under contradiction
		if (it->formula_ptr_ == nullptr) return;
		
		// mark was not inherited from parent
		if (it->print_mark_index_ == 0) {
			
			if (it->subtree_horizontal_length_ == 1) // variable -> mark starts on it's second print index
				it->print_mark_index_ = it->print_start_index_ + 1;
			else
				// formula -> set mark on middle of the formula, +1 'T' or 'F' flag character
				it->print_mark_index_ = it->print_start_index_ + (it->formula_ptr_->string_representation_.length() + 1) / 2;
		}
	
		// one child -> childs inherits mark position
		if (it->childs_.size() == 1) {

			it->childs_[0]->print_mark_index_ = it->print_mark_index_;
		}
	
		// recursively continue
		for (int i = 0; i < it->childs_.size(); i++) {
			calculate_mark_line_offsets(it->childs_[i].get());
		}
	}
}
