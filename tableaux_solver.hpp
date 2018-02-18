#ifndef  TABLEAUX_SOLVER_HPP_
#define  TABLEAUX_SOLVER_HPP_
#include <ostream>
#include <string>
#include <vector>
#include <queue>
#include "formula.hpp"
#include "formula_parser.hpp"

namespace tableaux {

	class solver {

	public:
		void solve(std::istream& input, std::ostream& output);
		void add_axiom(const std::string& formula);
		void add_root_formula(const std::string& root_formula, bool root_truth_value);

	private:
		bool get_truth_value(const char value) {
		
			if (value == 'F') return false;
			else if (value == 'T') return true;
			else throw std::invalid_argument("Input format error");
		}

		formula_parser parser_;

		formula_ptr root_formula_;
		bool root_formula_truth_value_;
		
		std::vector<formula_ptr> theory_axioms_;
		tableaux_branch initial_branch_;
	};
}

#endif
