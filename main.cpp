#include "tableaux_solver.hpp"
#include <iostream>
#include <fstream>
#include <istream>

using namespace std;

void print_usage() {
	
	cout << "Usage: tableaux_solver [options]" << endl;
	cout << "options (not required):" << endl;
	cout << "	-i [filename]    Name of input file. (implicit standard input)" << endl;
	cout << "	-o [filename]    Name of output file. (implicit standard output)" << endl;
}

// Parsing arguments from command line. Throws invalid_argument exception in case of incorrect arguments.
void parse_args(const int argc, char* argv[], fstream& in, fstream& out) {
	
	// checking duplicities of options
	bool has_input_parsed = false;
	bool has_output_parsed = false;

	size_t i = 1;
	while (i != argc) {
		
		 const string arg = argv[i];

		if (arg == "-i") {

			if (has_input_parsed) 
				throw invalid_argument("Can't use parameter '-i' more than once.");
			if (i + 1 == argc)
				throw invalid_argument("Parameter -i can't be last parameter, input filename must be provided.");
			
			in.open(argv[i+1], fstream::in);
			if (in.fail()) 
				throw invalid_argument("Input file doesn't exist or can't be opened for reading.");

			has_input_parsed = true;
			i += 2;
		}

		else if (arg == "-o") {

			if (has_output_parsed) 
				throw invalid_argument("Can't use parameter '-o' more than once.");
			if (i + 1 == argc) 
				throw invalid_argument("Parameter -o can't be last parameter, output filename must be provided.");

			out.open(argv[i + 1], fstream::out);
			if (out.fail())
				throw invalid_argument("Output file can't be opened for writing.");

			has_output_parsed = true;
			i += 2;
		}

		else throw invalid_argument("Wrong parameter.");
	}
}

int main(const int argc, char* argv[]) {

	fstream input;
	fstream output;

	try {
		parse_args(argc, argv, input, output);
	}
	catch (const invalid_argument& invalid_arg) {

		cout << invalid_arg.what() << endl;
		print_usage();
		return 1;
	}

	using namespace tableaux;
	tableaux::solver solver;

	try {
		
		solver.solve(
			input.is_open() ? input : cin,
			output.is_open() ? output : cout
		);
	}
	catch (std::invalid_argument& e) {
		std::cout << e.what() << std::endl;
		return 1;
	}

	return 0;
}
