#ifndef TMPARSING_H
#define TMPARSING_H

#include "../LLParser/LLParser.h"
#include "../TuringMachine/TuringMachine.h"
#include "ParseTree.h"
#include <memory>

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

	// Temporary variables used during conversion
	vector<State> states;
	vector<State> finals;

	vector<std::string> inputSymbols;
	vector<std::string> tapeSymbols;

	vector<Transition> transitions;

	State current;
	bool buildOnCurrent = true;
	std::string blank;

	std::vector<ParseTreeNode*> functions;

	// Keep counts to enforce uniqueness
	// We could handle this with 1 counter but this is cleaner imo
	int writeCount = 0;
	int endCount = 0;
	int moveCount = 0;
	int ifCount = 0;
	int whileCount = 0;
	int funcCount = 0;

	TuringMachine getTM ();

	// Building blocks
	void handleBlock (ParseTreeNode* node);
	void handleStatement (ParseTreeNode* node);

	// Symbol parsing
	void handleList (ParseTreeNode* node);

	// Control structure
	void handleWhile (ParseTreeNode* node);
	void handleIf (ParseTreeNode* node);

	// The meat of the program
	void handleMove (ParseTreeNode* node);
	void handleEndstate (ParseTreeNode* node);
	void handleWrite (ParseTreeNode* node);

	// Functions
	void handleFunctionCall (ParseTreeNode* node);
	void handleFunctionDefinition (ParseTreeNode* node);

	vector< std::string > getOtherSymbols (vector< std::string > list);
	ParseTreeNode* getFunctionNode (std::string name);
};

#endif