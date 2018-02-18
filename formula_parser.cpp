#include "formula_parser.hpp"
#include "formula_conjunction.hpp"
#include "formula_disjunction.hpp"
#include "formula_equivalence.hpp"
#include "formula_implication.hpp"
#include "formula_negation.hpp"
#include "formula_variable.hpp"

namespace tableaux {

	formula_ptr formula_parser::parse_formula(const std::string& unparsed_formula) {

		const auto tokens = get_formula_tokens(unparsed_formula);
		return assemble_formula(tokens);
	}

	tokens formula_parser::get_formula_tokens(const std::string& unparsed_formula) {

		tokens tokens;
		auto it = unparsed_formula.begin();
		const auto end = unparsed_formula.end();

		while (it != end) {

			auto current_token = parse_token::try_get_token_and_iterate_over(it, end);
			if (!current_token.empty()) tokens.push_back(current_token);
		}

		return tokens;
	}

	formula_ptr formula_parser::assemble_formula(const tokens& tokens) {

		if (tokens.size() == 3) return get_binary_formula(tokens);
		if (tokens.size() == 2) return get_unary_formula(tokens);
		if (tokens.size() == 1 && tokens[0].get_type() == parse_token::type::variable) return get_variable(tokens[0]);
		if (tokens.size() == 1 && tokens[0].get_type() == parse_token::type::subformula) return get_subformula(tokens[0]);
		
		throw std::invalid_argument("Input format error");
	}

	formula_ptr formula_parser::get_binary_formula(const tokens& tokens) {

		formula_ptr lhs;
		formula_ptr rhs;

		if (tokens[0].get_type() == parse_token::type::subformula) lhs = get_subformula(tokens[0]);
		else if (tokens[0].get_type() == parse_token::type::variable) lhs = get_variable(tokens[0]);
		if (tokens[2].get_type() == parse_token::type::subformula) rhs = get_subformula(tokens[2]);
		else if (tokens[2].get_type() == parse_token::type::variable) rhs = get_variable(tokens[2]);

		if (tokens[1].get_value() == "IMP") return std::make_unique<formula_implication>(std::move(rhs), std::move(lhs));
		else if (tokens[1].get_value() == "EKV") return std::make_unique<formula_equivalence>(std::move(rhs), std::move(lhs));
		else if (tokens[1].get_value() == "AND") return std::make_unique<formula_conjunction>(std::move(rhs), std::move(lhs));
		else if (tokens[1].get_value() == "OR") return std::make_unique<formula_disjunction>(std::move(rhs), std::move(lhs));

		throw std::invalid_argument("Input format error");
	}

	formula_ptr formula_parser::get_unary_formula(const tokens& tokens) {
	
		formula_ptr subformula;
		if (tokens[0].get_type() == parse_token::type::junction && tokens[0].get_value() == "NOT") {
			
			if (tokens[1].get_type() == parse_token::type::subformula) subformula = get_subformula(tokens[1]);
			else if (tokens[1].get_type() == parse_token::type::variable) subformula = get_variable(tokens[1]);
			else throw std::invalid_argument("Input format error");

			return std::make_unique<formula_negation>(std::move(subformula));
		}

		else throw std::invalid_argument("Input format error");
	}

	formula_ptr formula_parser::get_subformula(const parse_token& parse_token) {

		return parse_formula(parse_token.get_value());
	}

	formula_ptr formula_parser::get_variable(const parse_token& parse_token) {

		return std::make_unique<formula_variable>(parse_token.get_value()[0]);
	}

}
