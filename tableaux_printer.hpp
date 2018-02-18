#ifndef  TABLEAUX_PRINTER_HPP_
#define  TABLEAUX_PRINTER_HPP_
#include "formula.hpp"
#include <ostream>

namespace tableaux {

	class tableaux_printer {

	public:
		static void print_tableaux(tableaux_tree_node* root, std::ostream& output);
	private:
		// calculates horizotnal length of subtree with node as root for printing
		static size_t calculate_subtree_block_length(tableaux_tree_node* root);
		
		static void calculate_formula_line_offsets(tableaux_tree_node* root, size_t start, size_t end);
		static void calculate_mark_line_offsets(tableaux_tree_node* root);
		static const size_t gap_ = 10; // length of gap between forked branches
	};
}

#endif
