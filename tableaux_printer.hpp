#ifndef  TABLEAUX_PRINTER_HPP_
#define  TABLEAUX_PRINTER_HPP_
#include "formula.hpp"
#include <ostream>

namespace tableaux {

	class tableaux_printer {

	public:
		static void print_tableaux(item* root, std::ostream& output);
	private:
		static size_t calculate_bounds(item* root);
		static void calculate_formula_offsets(item* root, size_t start, size_t end);
		static void calculate_mark_offsets(item* root);
	};
}

#endif
