#include "TMParsing.h"
#include "../TuringMachine/TuringMachine.h"

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
	this->finals = {accept};

	this->inputSymbols = {};
	this->tapeSymbols = {};

	this->transitions = {};

	this->current = State("start");
	this->blank = std::string("HIDDEN_BLANK_LABEL");

	// Fill in the turing machine propertys from the tree
	this->handleBlock(this->parseTree->root);

	return TuringMachine(this->states,
	                     this->inputSymbols,
	                     this->tapeSymbols,
	                     this->transitions,
	                     State("start"),
	                     this->blank,
	                     this->finals,
	                     Tape tape());
}

void TMParser::handleBlock (Node* node) {
	if (node->value != std::string("B"))
		throw std::runtime_error("HandleBlock did not get a block.");

	for (auto& child : node->children) {
		if (child->value == std::string("B"))
			this->handleBlock(child);
		else if (child->value == std::string("S"))
			this->handleStatement(child);
		else
			throw std::runtime_error("Block had non block or statement child");
	}
}

void TMParser::handleStatement (Node* node) {
	if (node->value != std::string("S"))
		throw std::runtime_error("handleStatement did not get a statement.");

	Node* child = node->children.at(0);
	if (child->value == std::string("l")
		this->handleList(child);
	else if (child->value == std::string("w"))
		this->handleWhile(node);
	else if (child->value == std::string("i"))
		this->handleIf(node);
	else if (child->value == std::string("m") || child->value == std::string("r"))
		this->handleMove(child);
	else if (child->value == std::string("a") || child->value == std::string("n"))
		this->handleEndstate(child);
	else if (child->value == std::string("c"))
		this->handleFunctionCall(child);
	else if (child->value == std::string("d"))
		this->handleFunctionDefinition(child);
	else
		throw std::runtime_error("Statement had unknown child: " + child->value);
}

void TMParser::handleList (Node* node) {
	for (auto& symbol : node->name) {
		this->inputSymbols.push(symbol);
		this->tapeSymbols.push(symbol);
	}
}

void TMParser::handleWhile (Node* node) {
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

	this->states.push(whileState);
	this->states.push(whileEndState);

	// Handle the body
	this->current = whileState;
	this->handleBlock(node->children.at(2));

	// Get the state we are in if we went in the whileloop
	State inWhileEnd = this->current;

	// Create transitions for the nodes in the list, to the whilestate
	// And from the whilestateend back to the whilestate
	Node* list = node->children.at(1);
	for (auto& symbol : node->name) {
		Transition temp = Transition(previous, whileState, "N", symbol, "");
		this->transitions.push(temp);


		temp = Transition(inWhileEnd, whileState, "N", symbol, "");
		this->transitions.push(temp);
	}

	// With the other symbols, go to the end
	// Both in the while and before the while
	vector<std::string> symbolsNotInList = this->getOtherSymbols(node->name);
	for (auto& symbol : symbolsNotInList) {
		Transition temp = Transition(previous, whileEndState, "N", symbol, "");
		this->transitions.push(temp);

		temp = Transition(inWhildEnd, whileEndState, "N", symbol, "");
		this->transitions.push(temp);
	}

	// Build from the endstate
	this->current = whileEndState;
}

void TMParser::handleIf (Node* node) {
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

	this->states.push(ifState);
	this->states.push(ifEndState);

	// Handle the body
	this->current = ifState;
	this->handleBlock(node->children.at(2));

	// Get the state we are in if we went in the ifblock
	State inIfEnd = this->current;

	// Create transitions for the nodes in the list, to the ifstate
	// And from the end state in the if to the end state out of the if
	Node* list = node->children.at(1);
	for (auto& symbol : node->name) {
		Transition temp = Transition(previous, ifState, "N", symbol, "");
		this->transitions.push(temp);


		temp = Transition(inIfEnd, ifEndState, "N", symbol, "");
		this->transitions.push(temp);
	}

	// With the other symbols, go to the end
	// Both in the if and outside of it
	vector<std::string> symbolsNotInList = this->getOtherSymbols(node->name);
	for (auto& symbol : symbolsNotInList) {
		Transition temp = Transition(previous, ifEndState, "N", symbol, "");
		this->transitions.push(temp);

		temp = Transition(inIfEnd, ifEndState, "N", symbol, "");
		this->transitions.push(temp);
	}

	// Build from the endstate
	this->current = ifEndState;
}

void TMParser::handleMove (Node* node) {
	this->moveCount++;

	State next = State(std::string("move") + std::to_string(this->moveCount));
	std::string dir = std::string("R");

	if (node->value == std::string("m"))
		dir = std::string("L");

	for (auto& symbol : this->tapeSymbols) {
		Transition temp = Transition(this->current, next, dir, symbol, "");
		this->transitions.push(temp);
	}

	this->current = next;
}

void TMParser::handleEndstate (Node* node) {
	this->endCount++;

	State next = State(std::string("end") + std::to_string(this->endCount));

	State end = State("accept");
	if (node->value == "n") end = State("reject");

	for (auto& symbol : this->tapeSymbols) {
		Transition temp = Transition(this->current, end, "N", symbol, "");
		this->transitions.push(temp);
	}

	this->current = next;
}

void TMParser::handleFunctionCall (Node* node) {
	
}

void TMParser::handleFunctionDefinition (Node* node) {
	
}