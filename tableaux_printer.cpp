#include "tableaux_printer.hpp"
#include <ostream>

namespace tableaux {

	void tableaux::tableaux_printer::print_tableaux(item* root, std::ostream& output) {

		calculate_bounds(root);
		calculate_formula_offsets(root, 0, root->print_length);
		calculate_mark_offsets(root);

		std::vector<item*> bfs_queue;
		bfs_queue.push_back(root);
		std::string line_formulas(root->print_length, ' ');
		std::string line_marks(root->print_length, ' ');
		std::string line_connections(root->print_length, ' ');

		while (!bfs_queue.empty()) {

			std::vector<item*> new_items;

			for (const auto& item : bfs_queue) {

				if (item->formula_ptr != nullptr) {

					std::string formula;
					if (item->formula_ptr->string_representation.length() == 1) {
						formula = (item->truth_value) ? "T(" + item->formula_ptr->string_representation + ')' :
							"F(" + item->formula_ptr->string_representation + ')';
					}
					else {
						formula = (item->truth_value) ? 'T' + item->formula_ptr->string_representation :
							'F' + item->formula_ptr->string_representation;
					}

					for (int i = 0; i < formula.length(); i++) {

						line_formulas[item->print_start_index + i] = formula[i];
					}

					if (item->childs.size() != 0) {
						line_marks[item->print_mark_index] = '|';

						if (item->childs.size() == 2)
							for (int i = item->childs[0]->print_mark_index; i <= item->childs[1]->print_mark_index; i++) {
								line_connections[i] = '-';
							}
						else line_connections[item->print_mark_index] = '|';

					}
				}
				else {
					line_formulas[item->parent->print_mark_index] = 'X';
					//line_marks[item->print_start_index] = '|';
				}

				for (int i = 0; i < item->childs.size(); i++) {

					new_items.push_back(item->childs[i].get());
				}

			}

			output << line_formulas.c_str() << std::endl << line_marks.c_str() << std::endl <<
					  line_connections.c_str() << std::endl;
			//std::cout << line_marks;
			line_formulas = line_marks = line_connections = std::string(root->print_length, ' '); // fix
			bfs_queue = new_items; 
		}
	}

	// Recursive function
	size_t tableaux::tableaux_printer::calculate_bounds(item* it) {

		// Has no childs --> end of branch & is variable or contradiction mark
		if (it->childs.size() == 0) {

			it->print_length = 4;
			return 4;
		}
		// Length of subtree, will be compared with length of current formula
		size_t subtree_print_length = 0;

		for (int i = 0; i < it->childs.size(); i++) {

			// Child's formula is nullptr -> contradiction
			if (it->childs[i]->formula_ptr == nullptr) {

				it->childs[i]->print_length = 1;
				subtree_print_length += it->childs[i]->print_length;
			}
			// Else take size of subtree under child item
			else subtree_print_length += calculate_bounds(it->childs[i].get());
		}

		// For each gap between childs add 10 characters
		subtree_print_length += 10 * (it->childs.size() - 1);

		// Size of formula with tableaux wrapper
		size_t tableaux_formula_length = 0;
		if (it->formula_ptr->string_representation.length() == 1) // variable
			tableaux_formula_length = 4;  // T/F + parenthesis
		else
			tableaux_formula_length = it->formula_ptr->string_representation.length() + 1; // just T/F flag, parentheses already included

		it->print_length = tableaux_formula_length > subtree_print_length ?
			tableaux_formula_length : subtree_print_length;

		return it->print_length;
	}

	void tableaux::tableaux_printer::calculate_formula_offsets(item* it, size_t start, size_t end) {

		size_t offset = 0;
		if (it->formula_ptr != nullptr) {
			
			if (it->formula_ptr->string_representation.length() == 1)
				offset = ((end - start) - (it->formula_ptr->string_representation.length() + 3)) / 2;
			else {
				offset = ((end - start) - (it->formula_ptr->string_representation.length() + 1)) / 2;
			}
		}
		else { // contradiction
			it->print_start_index = start;
			return;
		}

		// handling childs

		it->print_start_index = start + offset;

		// get total print count of subtree
		size_t subtree_count = 0;
		for (int i = 0; i < it->childs.size(); i++) {

			subtree_count += it->childs[i]->print_length;
		}
		subtree_count += 10 * (it->childs.size() - 1);

		// get start offset for printing subtree items
		offset = ((end - start) - subtree_count) / 2;

		for (int i = 0; i < it->childs.size(); i++) {

			calculate_formula_offsets(it->childs[i].get(), offset + start, offset + start + it->childs[i]->print_length);
			offset += 10 + it->childs[i]->print_length;
		}
	}

	void tableaux::tableaux_printer::calculate_mark_offsets(item* it) {
	
		if (it->formula_ptr == nullptr) return;
		
		if (it->print_mark_index == 0) {
			
			if (it->print_length == 1)
				it->print_mark_index = it->print_start_index + 1;
			else
				it->print_mark_index = it->print_start_index + (it->formula_ptr->string_representation.length() + 1) / 2;
		}
	
		if (it->childs.size() == 1) {

			it->childs[0]->print_mark_index = it->print_mark_index;
		}
	
		for (int i = 0; i < it->childs.size(); i++) {
			calculate_mark_offsets(it->childs[i].get());
		}
	}
}
