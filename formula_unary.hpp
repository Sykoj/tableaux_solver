#ifndef  FORMULA_UNARY_HPP_
#define  FORMULA_UNARY_HPP_
#include "formula.hpp"

namespace tableaux {

	class formula_unary : public formula {

	protected:
		formula_ptr subformula_;
	};
}

#endif
