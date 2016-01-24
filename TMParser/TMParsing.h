#ifndef TMPARSING_H
#define TMPARSING_H

#include "../LLParser/LLParser.h"
#include "ParseTree.h"

// TMPARSING.H
//--------------------------------------------------------------------------

class TMParser {
  public:
	TMParser(std::string p, std::string c);

	std::string derivation;
	std::vector<std::string> derivations;
	std::vector<std::string> replacements;
	bool correctlyParsed;
	ParseTree* parseTree;
};

#endif