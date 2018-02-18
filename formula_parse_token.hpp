#ifndef  FORMULA_PARSE_TOKEN_HPP_
#define  FORMULA_PARSE_TOKEN_HPP_
#include <string>

namespace tableaux {

	class parse_token {

		using string_const_iter = std::string::const_iterator;

	public:
		enum class type { subformula, variable, empty, junction };
		
		parse_token() = default;
		parse_token(const std::string& value, const type type) : value_(value), type_(type) {}

		std::string get_value() const { return value_; }
		type get_type() const { return type_; }
		static parse_token try_get_token_and_iterate_over(string_const_iter& iter, const string_const_iter& end);
		bool empty() const { return (type_ == type::empty); }

	private:
		static parse_token get_unparsed_subformula_and_iterate_through(string_const_iter& iter, const string_const_iter& end);
		static parse_token get_junction_and_iterate_through(string_const_iter& iter, const string_const_iter& end);
		static parse_token get_variable_and_iterate_through(string_const_iter& iter);
		static parse_token get_empty_token_and_iterate_through(string_const_iter& iter);
		std::string value_;
		type type_ = type::empty;
	};
}

#endif
