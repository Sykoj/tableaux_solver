#include "token.hpp"

namespace tableaux {

	token token::try_get_token_and_iterate_over(string_const_iter& it, const string_const_iter& end) {

		if (*it == '(') return get_unparsed_subformula_and_iterate_through(it, end);
		if (isupper(*it)) return get_junction_and_iterate_through(it, end);
		if (islower(*it)) return get_variable_and_iterate_through(it);
		if (*it == ' ') return get_empty_token_and_iterate_through(it);

		throw std::invalid_argument("Input format error");
	}

	token token::get_unparsed_subformula_and_iterate_through(string_const_iter& iter, const string_const_iter& end) {
		
		size_t parenthesis_depth = 0;
		const auto begin = iter;

		do {
			if (*iter == '(') ++parenthesis_depth;
			if (*iter == ')') --parenthesis_depth;
			++iter;
			if (iter == end && parenthesis_depth > 0) throw std::invalid_argument("Input format error");
		} while (parenthesis_depth > 0);

		// +1, -1 to remove outer parentheses
		return token(std::string(begin + 1, iter - 1), type::subformula);
	}

	token token::get_junction_and_iterate_through(string_const_iter& iter, const string_const_iter& end) {
		
		const auto begin = iter;
		while (isupper(*iter)) {
			++iter; if (iter == end) break;
		}
		return token(std::string(begin, iter), type::junction);
	}

	class token token::get_variable_and_iterate_through(string_const_iter& iter) {
		
		const auto variable = *iter;
		++iter; return token(std::string(1, variable), type::variable);
	}

	token token::get_empty_token_and_iterate_through(string_const_iter& iter) {
		
		++iter; return token{};
	}
}
