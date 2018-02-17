#ifndef  FORMULA_BINARY_HPP_
#define  FORMULA_BINARY_HPP_
#include "formula.hpp"

namespace tableaux {

	class formula_binary : public formula {
	protected:
		formula_ptr rhs_; // right-hand side of binary formula
		formula_ptr lhs_; // left-hand side of binary formula
	};
}

#endif
