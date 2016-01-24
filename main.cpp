#include "TMParser/TMParsing.h"

int main() {
	// Dees is alles da ge moet doen
	TMParser tm = TMParser("program.tm", "CFG.xml");

	// Kijk wa een coole dinge da ge ermee kunt doen
	if (tm.correctlyParsed) {
		std::cout << tm.derivation << std::endl;
	}
	else {
		std::cout << "DIKKE PECH TIS MISLUKT - laurence fix pls" << std::endl;
	}

	// gg super veel werk
	// seriously ik moet meer doen dan dees anders ben ik nisse 2
	return 0;
}