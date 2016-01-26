#include "TMParsing.h"
#include "../TuringMachine/TuringMachine.h"
#include "ParseTree.h"
#include <exception>

TMParser::TMParser(std::string p, std::string c) {
	correctlyParsed = CfgToPdaAndTest(p, c, derivations, replacements);

	if (correctlyParsed) {
		derivation = derivations.back();
	}

	parseTree = new ParseTree(derivations, replacements);
}

// Get the turing machine from the parse tree
TuringMachine TMParser::getTM () {
	this->states = {State("start"), State("accept"), State("reject")};
	this->finals = {State("accept")};

	this->inputSymbols = {};
	this->tapeSymbols = {};

	this->transitions = {};

	this->current = State("start");
	this->buildOnCurrent = true;
	this->blank = std::string("BLANK");

	// Fill in the turing machine propertys from the tree
	this->handleBlock(this->parseTree->root->children.at(0));

	return TuringMachine(this->states,
	                     this->inputSymbols,
	                     this->tapeSymbols,
	                     this->transitions,
	                     State("start"),
	                     this->blank,
	                     this->finals,
	                     Tape());
}

void TMParser::handleBlock (ParseTreeNode* node) {
	if (node->value != std::string("B"))
		throw std::runtime_error((std::string("HandleBlock did not get a block. Got: ") + node->value).c_str());

	for (auto& child : node->children) {
		if (child->value == std::string("B"))
			this->handleBlock(child);
		else if (child->value == std::string("S"))
			this->handleStatement(child);
		else if (child->value == std::string("y"));
		else
			throw std::runtime_error((std::string("Block had a none Block or Statement child. Got: ") + child->value).c_str());
	}
}

void TMParser::handleStatement (ParseTreeNode* node) {
	if (node->value != std::string("S"))
		throw std::runtime_error((std::string("HandleStatement did not get a statement. Got: ") + node->value).c_str());

	ParseTreeNode* child = node->children.at(0);
	if (child->value == std::string("l"))
		this->handleList(child);
	else if (child->value == std::string("w"))
		this->handleWhile(node);
	else if (child->value == std::string("i"))
		this->handleIf(node);
	else if (child->value == std::string("m") || child->value == std::string("r"))
		this->handleMove(child);
	else if (child->value == std::string("a") || child->value == std::string("n"))
		this->handleEndstate(child);
	else if (child->value == std::string("z"))
		this->handleWrite(child);
	else if (child->value == std::string("c"))
		this->handleFunctionCall(node);
	else if (child->value == std::string("d"))
		this->handleFunctionDefinition(node);
	else if (child->value == std::string("y"));
	else
		throw std::runtime_error("Statement had unknown child: " + child->value);
}

void TMParser::handleList (ParseTreeNode* node) {
	for (auto& symbol : node->name) {
		this->inputSymbols.push_back(symbol);
		this->tapeSymbols.push_back(symbol);
	}
}

void TMParser::handleWhile (ParseTreeNode* node) {
	if (!this->buildOnCurrent) return;
	State previous = this->current;
	this->whileCount++;

	// Create a new state for the while body
	// And for the whileend
	State whileState = State(std::string("while") +
	                         std::to_string(this->whileCount));

	// The state we return to after we went in the while
	// Or if we bypass the while
	State whileEndState = State(std::string("whileEnd") +
	                            std::to_string(this->whileCount));

	this->states.push_back(whileState);
	this->states.push_back(whileEndState);

	// Handle the body
	this->current = whileState;
	this->buildOnCurrent = true;
	this->handleBlock(node->children.at(2));

	// Get the state we are in if we went in the whileloop
	State inWhileEnd = this->current;

	// Create transitions for the nodes in the list, to the whilestate
	// And from the whilestateend back to the whilestate
	ParseTreeNode* list = node->children.at(1);

	if (list->value != std::string("l"))
		throw runtime_error("While had non list child at index 1. Got: " + list->value);

	for (auto& symbol : list->name) {
		Transition temp = Transition(previous, whileState, "N", symbol, "");
		this->transitions.push_back(temp);


		if (this->buildOnCurrent) {
			temp = Transition(inWhileEnd, whileState, "N", symbol, "");
			this->transitions.push_back(temp);
		}
	}

	// With the other symbols, go to the end
	// Both in the while and before the while
	vector<std::string> symbolsNotInList = this->getOtherSymbols(list->name);
	for (auto& symbol : symbolsNotInList) {
		Transition temp = Transition(previous, whileEndState, "N", symbol, "");
		this->transitions.push_back(temp);

		if (this->buildOnCurrent) {
			temp = Transition(inWhileEnd, whileEndState, "N", symbol, "");
			this->transitions.push_back(temp);
		}
	}

	// Build from the endstate
	this->current = whileEndState;
	this->buildOnCurrent = true;
}

void TMParser::handleIf (ParseTreeNode* node) {
	if (!this->buildOnCurrent) return;
	State previous = this->current;
	this->ifCount++;

	// Create a new state for the if body
	// And for the ifend
	State ifState = State(std::string("if") +
	                      std::to_string(this->ifCount));

	// The state we return to after we went in the if
	// Or if we bypass the if
	State ifEndState = State(std::string("ifEnd") +
	                         std::to_string(this->ifCount));

	this->states.push_back(ifState);
	this->states.push_back(ifEndState);

	// Handle the body
	this->current = ifState;
	this->buildOnCurrent = true;
	this->handleBlock(node->children.at(2));

	// Get the state we are in if we went in the ifblock
	State inIfEnd = this->current;

	// Create transitions for the nodes in the list, to the ifstate
	// And from the end state in the if to the end state out of the if
	ParseTreeNode* list = node->children.at(1);

	if (list->value != std::string("l"))
		throw runtime_error("While had non list child at index 1. Got: " + list->value);

	for (auto& symbol : list->name) {
		Transition temp = Transition(previous, ifState, "N", symbol, "");
		this->transitions.push_back(temp);

		if (this->buildOnCurrent) {
			temp = Transition(inIfEnd, ifEndState, "N", symbol, "");
			this->transitions.push_back(temp);
		}
	}

	// With the other symbols, go to the end
	// Both in the if and outside of it
	vector<std::string> symbolsNotInList = this->getOtherSymbols(list->name);
	for (auto& symbol : symbolsNotInList) {
		Transition temp = Transition(previous, ifEndState, "N", symbol, "");
		this->transitions.push_back(temp);

		if (this->buildOnCurrent) {
			temp = Transition(inIfEnd, ifEndState, "N", symbol, "");
			this->transitions.push_back(temp);
		}
	}

	// Build from the endstate
	this->current = ifEndState;
	this->buildOnCurrent = true;
}

void TMParser::handleMove (ParseTreeNode* node) {
	if (!this->buildOnCurrent) return;
	this->moveCount++;

	State next = State(std::string("move") + std::to_string(this->moveCount));
	std::string dir = std::string("R");

	if (node->value == std::string("m"))
		dir = std::string("L");

	for (auto& symbol : this->tapeSymbols) {
		Transition temp = Transition(this->current, next, dir, symbol, "");
		this->transitions.push_back(temp);
	}

	this->current = next;
	this->buildOnCurrent = true;
}

void TMParser::handleEndstate (ParseTreeNode* node) {
	if (!this->buildOnCurrent) return;

	State end = State("accept");
	if (node->value == "n") end = State("reject");

	for (auto& symbol : this->tapeSymbols) {
		Transition temp = Transition(this->current, end, "N", symbol, "");
		this->transitions.push_back(temp);
	}

	this->buildOnCurrent = false;
}

void TMParser::handleWrite (ParseTreeNode* node) {
	if (!this->buildOnCurrent) return;
	this->writeCount++;

	State next = State(std::string("write") + std::to_string(this->writeCount));

	for (auto& symbol : this->tapeSymbols) {
		Transition temp = Transition(this->current, next, "N", symbol, node->name.at(0));
		this->transitions.push_back(temp);
	}

	this->current = next;
	this->buildOnCurrent = true;
}

void TMParser::handleFunctionCall (ParseTreeNode* node) {
	if (!this->buildOnCurrent) return;
	ParseTreeNode* functionNode = this->getFunctionNode(
		node->children.at(0)->name.at(0));

	if (functionNode == NULL)
		// Called function that was not (yet) defined
		throw std::runtime_error("You have called a function that was not (yet) defined: " + node->children.at(0)->name.at(0));

	this->funcCount++;

	State start = State(std::string("function_") +
	                   node->children.at(0)->name.at(0) + std::string("_") +
	                   std::to_string(this->funcCount));

	State next = State(std::string("function_") +
	                   node->children.at(0)->name.at(0) + std::string("_") +
	                   std::to_string(this->funcCount) + std::string("_end"));

	for (auto& symbol : this->tapeSymbols) {
		Transition temp = Transition(this->current, start, "N", symbol, "");
		this->transitions.push_back(temp);
	}

	this->states.push_back(start);
	this->states.push_back(next);
	this->current = start;

	this->handleBlock(functionNode->children.at(1));

	for (auto& symbol : this->tapeSymbols) {
		Transition temp = Transition(this->current, next, "N", symbol, "");
		this->transitions.push_back(temp);
	}

	this->current = next;
	this->buildOnCurrent = true;
	
}

void TMParser::handleFunctionDefinition (ParseTreeNode* node) {
	if (this->getFunctionNode(node->children.at(0)->name.at(0)) != NULL)
		throw std::runtime_error("Redefinition of " + node->children.at(0)->name.at(0));

	this->functions.push_back(node);
}

ParseTreeNode* TMParser::getFunctionNode (std::string name) {
	for (auto& node : this->functions) {
		if (node->children.at(0)->name.at(0) == name) return node;
	}

	return NULL;
}

vector< std::string > TMParser::getOtherSymbols (vector< std::string > list) {
	vector <std::string> copy = this->tapeSymbols;

	for (auto& other : list) {
		copy.erase(std::remove(copy.begin(), copy.end(), other), copy.end());
	}

	return copy;
}
