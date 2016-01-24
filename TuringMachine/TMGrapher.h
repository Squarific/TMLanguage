#ifndef TMGrapher_H
#define TMGrapher_H

#include "TuringMachine.h"
#include <string>

class TMGrapher {
public:
	TuringMachine tm;

	TMGrapher();
	TMGrapher(TuringMachine& tm);
	std::string toDot();
};

#endif