#include <iostream>
#include <fstream>

#include "TuringMachine.h"
#include "Parser.h"
#include "TMGrapher.h"

using namespace std;

// Arguments
// [NAME] INPUTFILE STRINGTOTEST OUTPUTFILE
int main (int argc, char *argv[]) {
	if (argc < 3) {
		cerr << "ERROR: Please provide at least 2 arguments. The file and a string to test. Example: TuringMachine TM.XML somestring" << endl;
		return 1;
	}

	Parser p = Parser();
	TuringMachine tm = p.TMFromXML(argv[1]);

	if (argc > 3) {
		TMGrapher tmg = TMGrapher(tm);

		std::ofstream file;
		file.open(argv[3]);

		file << tmg.toDot();
	}

	tm.currentTape = Tape(tm.blank, argv[2]);
	tm.run();

	cout << tm.accepted();

	return 0;
}