#ifndef  FORMULA_HPP_
#define  FORMULA_HPP_

#include <memory>
#include <vector>
#include <queue>
#include <set>
#include <ostream>
#include <string>

#include "branch.hpp"

namespace tableaux {

	class formula {

	public:
		virtual std::vector<tableaux::branch> atomic_fork(branch& previous, const item& developed) const = 0;
		
		virtual const std::string& set_string_representation() = 0;
		std::string string_representation;
		virtual ~formula() {};
	};

	using formula_ptr = std::unique_ptr<formula>;
}

#endif
