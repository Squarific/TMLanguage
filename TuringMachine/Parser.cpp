#include <iostream>
#include <vector>
#include <string>

#include "Parser.h"
#include "TuringMachine.h"

#include "./TinyXML/tinyxml.h"

TuringMachine Parser::TMFromXML(const char* filename) {
	TiXmlDocument doc(filename);

	if (!doc.LoadFile()) {
		std::cerr << "ERROR: Failed to load XML file." << std::endl;
		return TuringMachine();
	}

	TiXmlElement* root = doc.FirstChildElement();

	if (std::string(root->Value()) != "TM") {
		std::cerr << "ERROR: Root element was not TM." << std::endl;
		return TuringMachine();
	}


	std::vector<State> states = {};
	std::vector<std::string> inputSymbols = {};
	std::vector<std::string> tapeSymbols = {};
	std::vector<Transition> transitions = {};
	State start;
	std::string blank;
	std::vector<State> finals = {};

	for (TiXmlElement* element = root->FirstChildElement();
	     element != NULL; element = element->NextSiblingElement()) {

		std::string elementName = element->Value();

		if (elementName == "InputAlphabet") {
			inputSymbols = this->parseAlphabet(element);
		} else if (elementName == "TapeAlphabet") {
			tapeSymbols = this->parseAlphabet(element);
		} else if (elementName == "Blank") {
			blank = std::string(element->FirstChild()->ToText()->Value());
		} else if (elementName == "States") {
			states = this->parseStates(element);
		} else if (elementName == "Transitions") {
			transitions = this->parseTransitions(element);
		} else if (elementName == "StartState") {
			start = State(std::string(element->FirstChild()->FirstChild()->ToText()->Value()));
		} else if (elementName == "AcceptingStates") {
			finals = this->parseAcceptingStates(element);
		} else {
			std::cerr << "ERROR: Found node with unknown name: " << elementName << std::endl;
		}
	}

	Tape tape = Tape(blank);

	return TuringMachine(states, inputSymbols, tapeSymbols,
	                     transitions, start, blank, finals, tape);
}

std::vector<State> Parser::parseStates (TiXmlElement* element) {
	std::vector<State> tempvector = {};

	for (TiXmlElement* forElement = element->FirstChildElement();
	     forElement != NULL; forElement = forElement->NextSiblingElement()) {
		std::string elementName = forElement->Value();

		if (elementName == "state") {
			tempvector.push_back(State(forElement->FirstChild()->ToText()->Value()));
		} else {
			std::cerr << "ERROR: State list with unknown elementName: " << elementName << std::endl;
		}
	}

	return tempvector;
}

std::vector<State> Parser::parseAcceptingStates (TiXmlElement* element) {
	std::vector<State> tempvector = {};

	for (TiXmlElement* forElement = element->FirstChildElement();
	     forElement != NULL; forElement = forElement->NextSiblingElement()) {
		std::string elementName = forElement->Value();

		if (elementName == "state") {
			tempvector.push_back(State(forElement->FirstChild()->FirstChild()->ToText()->Value()));
		} else {
			std::cerr << "ERROR: State list with unknown elementName: " << elementName << std::endl;
		}
	}

	return tempvector;
}

std::vector<std::string> Parser::parseAlphabet (TiXmlElement* element) {
	std::vector<std::string> tempvector = {};

	for (TiXmlElement* forElement = element->FirstChildElement();
	     forElement != NULL; forElement = forElement->NextSiblingElement()) {
		std::string elementName = forElement->Value();

		if (elementName == "symbol") {
			tempvector.push_back(forElement->FirstChild()->ToText()->Value());
		} else {
			std::cerr << "ERROR: Symbol list with unknown elementName: " << elementName << std::endl;
		}
	}

	return tempvector;
}

std::vector<Transition> Parser::parseTransitions (TiXmlElement* element) {
	std::vector<Transition> tempvector = {};

	for (TiXmlElement* forElement = element->FirstChildElement();
	     forElement != NULL; forElement = forElement->NextSiblingElement()) {
		std::string elementName = forElement->Value();

		if (elementName == "transition") {
			tempvector.push_back(this->parseTransition(forElement));
		} else {
			std::cerr << "ERROR: Transition list with unknown elementName: " << elementName << std::endl;
		}
	}

	return tempvector;
}

Transition Parser::parseTransition (TiXmlElement* element) {
	State from;
	State to;

	std::string read;
	std::string write;
	std::string dir;

	for (TiXmlElement* forElement = element->FirstChildElement();
	     forElement != NULL; forElement = forElement->NextSiblingElement()) {
		std::string elementName = forElement->Value();

		if (elementName == "from") {
			from = State(forElement->FirstChild()->ToText()->Value());
		} else if (elementName == "to") {
			to = State(forElement->FirstChild()->ToText()->Value());
		} else if (elementName == "read") {
			read = forElement->FirstChild()->ToText()->Value();
		} else if (elementName == "write") {
			write = forElement->FirstChild()->ToText()->Value();
		} else if (elementName == "dir") {
			dir = forElement->FirstChild()->ToText()->Value();
		} else {
			std::cerr << "ERROR: Transition with unknown property: " << elementName << std::endl;
		}
	}

	return Transition(from, to, dir, read, write);
}