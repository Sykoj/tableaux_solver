
#ifndef ITEM_HPP_
#define ITEM_HPP_

#include <memory>
#include <vector>

namespace tableaux {

class formula;

class item {

	using formula_observer_ptr = const formula*;
	using item_ptr = std::unique_ptr<item>;

  public:
	item(formula_observer_ptr formula_ptr, const bool truth_value) : formula_ptr(formula_ptr), truth_value(truth_value) {}
	formula_observer_ptr formula_ptr;
	bool truth_value;
	item *parent = nullptr;
	std::vector<item_ptr> childs;

	size_t print_length = 0;
	size_t print_start_index = 0;
	//size_t print_end_index = 0;
	size_t print_mark_index = 0;

	item &operator=(const item &it)
	{

		formula_ptr = it.formula_ptr;
		truth_value = it.truth_value;
		print_length = it.print_length;
		parent = it.parent;
		childs.clear();
		return *this;
	}

	item &operator=(item &&item) noexcept
	{
		formula_ptr = item.formula_ptr;
		truth_value = item.truth_value;
		print_length = item.print_length;
		parent = item.parent;
		childs.clear();
		//childs = std::move(item.childs);
		return *this;
	}

	item(const item &it)
	{
		formula_ptr = it.formula_ptr;
		truth_value = it.truth_value;
		print_length = it.print_length;
		parent = it.parent;
		childs.clear();
	}

	item(item &&item)
	{
		formula_ptr = item.formula_ptr;
		truth_value = item.truth_value;
		print_length = item.print_length;
		parent = item.parent;
		childs.clear();
		//childs = std::move(item.childs);
	}
};
}

#endif
