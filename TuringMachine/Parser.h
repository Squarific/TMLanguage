#ifndef PARSER_H
#define PARSER_H

#include "TuringMachine.h"
#include "./TinyXML/tinyxml.h"

#include <vector>
#include <string>

class Parser {
public:
	// Gives back a turing machine
	// On errors, writes to cerr and returns an empty TM
	TuringMachine TMFromXML (const char* filename);

	std::vector<State> parseStates(TiXmlElement* element);
	std::vector<State> parseAcceptingStates(TiXmlElement* element);
	std::vector<std::string> parseAlphabet (TiXmlElement* element);

	std::vector<Transition> parseTransitions (TiXmlElement* element);
	Transition parseTransition (TiXmlElement* element);
};

#endif