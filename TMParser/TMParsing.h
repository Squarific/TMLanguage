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

	std::vector<ParseTreeNode*> functions;

	int writeCount = 0;
	int endCount = 0;
	int moveCount = 0;
	int ifCount = 0;
	int whileCount = 0;
};

#endif