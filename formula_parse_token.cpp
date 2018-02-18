#include "formula_parse_token.hpp"

namespace tableaux {

	parse_token parse_token::try_get_token_and_iterate_over(string_const_iter& it, const string_const_iter& end) {

		if (*it == '(') return get_unparsed_subformula_and_iterate_through(it, end);
		if (isupper(*it)) return get_junction_and_iterate_through(it, end);
		if (islower(*it)) return get_variable_and_iterate_through(it);
		if (*it == ' ') return get_empty_token_and_iterate_through(it);

		throw std::invalid_argument("Input format error");
	}

	parse_token parse_token::get_unparsed_subformula_and_iterate_through(string_const_iter& iter, const string_const_iter& end) {
		
		size_t parenthesis_depth = 0;
		const auto begin = iter;

		do {
			if (*iter == '(') ++parenthesis_depth;
			if (*iter == ')') --parenthesis_depth;
			++iter;
			if (iter == end && parenthesis_depth > 0) throw std::invalid_argument("Input format error");
		} while (parenthesis_depth > 0);

		// +1, -1 to remove outer parentheses
		return parse_token(std::string(begin + 1, iter - 1), type::subformula);
	}

	parse_token parse_token::get_junction_and_iterate_through(string_const_iter& iter, const string_const_iter& end) {
		
		const auto begin = iter;
		while (isupper(*iter)) {
			++iter; if (iter == end) break;
		}
		return parse_token(std::string(begin, iter), type::junction);
	}

	class parse_token parse_token::get_variable_and_iterate_through(string_const_iter& iter) {
		
		const auto variable = *iter;
		++iter; return parse_token(std::string(1, variable), type::variable);
	}

	parse_token parse_token::get_empty_token_and_iterate_through(string_const_iter& iter) {
		
		++iter; return parse_token{};
	}
}
