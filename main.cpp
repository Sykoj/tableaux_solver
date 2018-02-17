#include "tableaux_solver.hpp"
#include <iostream>

int main() {

	using namespace tableaux;
	tableaux::solver solver;

	try {
		solver.solve(std::cin, std::cout);
	}
	catch (std::invalid_argument e) {
		std::cout << e.what() << std::endl;
	}

	return 0;
}
