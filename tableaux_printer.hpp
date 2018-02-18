#ifndef  TABLEAUX_PRINTER_HPP_
#define  TABLEAUX_PRINTER_HPP_
#include "formula.hpp"
#include <ostream>

namespace tableaux {

	class tableaux_printer {

	public:
		static void print_tableaux(tableaux_tree_node* root, std::ostream& output);
	private:
		static size_t calculate_bounds(tableaux_tree_node* root);
		static void calculate_formula_offsets(tableaux_tree_node* root, size_t start, size_t end);
		static void calculate_mark_offsets(tableaux_tree_node* root);
	};
}

#endif
