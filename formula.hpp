#ifndef  FORMULA_HPP_
#define  FORMULA_HPP_

#include <memory>
#include <vector>
#include <queue>
#include <set>
#include <ostream>
#include <string>
#include "tableaux_branch.hpp"

namespace tableaux {

	class formula {

		friend class tableaux_printer;

	public:
		virtual std::vector<tableaux_branch> atomic_fork(tableaux_branch& previous, const tableaux_tree_node& developed) const = 0;
		virtual const std::string& set_string_representation() = 0;	
		virtual ~formula() {};
	protected:
		std::string string_representation_;
	};

	using formula_ptr = std::unique_ptr<formula>;
}

#endif