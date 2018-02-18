#include "tableaux_solver.hpp"
#include "formula_parser.hpp"
#include "tableaux_printer.hpp"
#include "formula.hpp"
#include "tableaux_node.hpp"
#include "tableaux_branch.hpp"
#include <queue>
#include <iostream>
#include <istream>
#include <ostream>

namespace tableaux {

	void solver::add_root_formula(const std::string& root_formula, bool root_truth_value) {
		
		root_formula_ = parser_.parse_formula(root_formula);
		root_formula_.get()->set_string_representation();
		root_formula_truth_value_ = root_truth_value;
	}

	void solver::add_axiom(const std::string& formula) {
		
		auto axiom_formula = parser_.parse_formula(formula);
		axiom_formula.get()->set_string_representation();
		theory_axioms_.push_back(std::move(axiom_formula));
	}

	void solver::solve(std::istream& input, std::ostream& output) {
		
		std::string line;
	
		getline(std::cin, line);

		if (line.empty()) throw std::invalid_argument("Input format error");
			add_root_formula(line.substr(1), get_truth_value(line[0]));

		while (getline(std::cin, line)) {

			if (line.empty()) throw std::invalid_argument("Input format error");
			add_axiom(line.substr(1));
		}

		auto root_node = std::make_unique<tableaux_tree_node>(root_formula_.get(), root_formula_truth_value_);

		// Start of tableaux method algorithm
		initial_branch_.head_ = root_node.get();
		for (const auto& axiom : theory_axioms_)
			initial_branch_.axiom_queue_.emplace(axiom.get(), true);

		std::queue<tableaux_branch> branches;
		branches.push(initial_branch_);

		while (!branches.empty()) {

			auto top = branches.front(); branches.pop();
			auto forked_branches = top.develop();

			for (const auto& branch : forked_branches) {
				branches.push(branch);
			}
		}
		// End of tableaux method algorithm
		
		tableaux_printer::print_tableaux(root_node.get(), output);
	}
}
