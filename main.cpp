#include "TMParser/TMParsing.h"
#include "TuringMachine/TuringMachine.h"
#include "TuringMachine/TMGrapher.h"
#include <iostream>
#include <exception>

// Arguments: SOURCE DOTOUTPUT
int main(int argc, char *argv[]) {
	if (argc < 3) {
		std::cerr << "You need to specify 2 arguments." << std::endl << std::endl;
		std::cerr << "First argument is the machine source file." << std::endl;
		std::cerr << "Example: program.tm" << std::endl << std::endl;
		std::cerr << "Second argument is the dot output file." << std::endl;
		std::cerr << "Example: tm.dot." << std::endl << std::endl;
		std::cerr << "Full example: Parser program.tm tm.dot" << std::endl << endl;
		return 1;
	}

	TMParser tm = TMParser(argv[1], "CFG.xml");

	if (!tm.correctlyParsed) {
		throw std::runtime_error("Parsing failed");
		return 1;
	}

	TuringMachine parsed = tm.getTM();

	TMGrapher tmg = TMGrapher(parsed);

	std::ofstream file;
	file.open(argv[2]);

	file << tmg.toDot();

	return 0;
}