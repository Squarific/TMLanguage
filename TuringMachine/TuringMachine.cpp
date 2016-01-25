#include "TuringMachine.h"

#include <vector>
#include <string>
#include <iostream>

State::State (std::string name) {
	this->name = name;
}

Transition::Transition (State from, State to, std::string headMove, std::string read, std::string write) {
	this->from = from;
	this->to = to;
	this->headMove = headMove;
	this->read = read;
	this->write = write;
}

std::string Tape::getCurrentSymbol () {
	if (this->head >= this->tape.size()) {
		this->tape.resize(this->head + 1, this->blank);
	}

	return this->tape.at(this->head);
}

void Tape::setCurrentSymbol (std::string symbol) {
	if (this->head >= this->tape.size()) {
		this->tape.resize(this->head + 1, this->blank);
	}

	this->tape.at(this->head) = symbol;
}

void Tape::moveLeft () {
	if (this->head == 1) {
		this->head = 0;
	} else if (this->head % 2 == 1) {
		this->head -= 2;
	} else {
		this->head += 2;
	}
}

void Tape::moveRight () {
	if (this->head == 0) {
		this->head = 1;
	} else if (this->head % 2 == 1) {
		this->head += 2;
	} else {
		this->head -= 2;
	}	
}

Tape::Tape (std::string blank) {
	this->blank = blank;
}

Tape::Tape (std::string blank, std::string startString) {
	this->blank = blank;

	for (auto& symbol : startString) {
		// std::string(1, char) reates a string by repeating char 1 time
		this->setCurrentSymbol(std::string(1, symbol));
		this->moveRight();
	}

	this->head = 0;
}

TuringMachine::TuringMachine(std::vector<State> states,
                             std::vector<std::string> inputSymbols,
                             std::vector<std::string> tapeSymbols,
                             std::vector<Transition> transitions,
                             State start,
                             std::string blank,
                             std::vector<State> finals,
                             Tape tape) {
	this->states = states;
	this->finals = finals;

	this->inputSymbols = inputSymbols;
	this->tapeSymbols = tapeSymbols;

	this->transitions = transitions;
	this->blank = blank;

	this->currentState = start;
	this->currentTape = tape;
}

bool TuringMachine::addModule (Transition trans, TuringMachine module, State& returnState) {
	std::string randomPrefix = std::to_string('a' + rand() % 26) +
	                           std::to_string('a' + rand() % 26) +
	                           std::to_string('a' + rand() % 26) +
	                           std::to_string('a' + rand() % 26) +
	                           std::to_string('a' + rand() % 26);

	this->transitions.push_back(trans);

	for (auto& trans : module.transitions) {
		std::string toState = randomPrefix + "_" + trans.to.name;

		// If this state goes to a final state, goto returnstate instead
		for (auto& finalTrans : module.finals) {
			if (finalTrans.name == trans.to.name) {
				toState = returnState.name;
				break;
			}
		}

		this->transitions.push_back(Transition(
			State(randomPrefix + "_" + trans.from.name),
			State(toState),
			trans.headMove,
			trans.read,
			trans.write
		));
	}

	return true;
}

bool TuringMachine::hasFinished () {
	// If we are in an accept state we are finished
	if (this->accepted()) return true;

	// Get the current symbol
	std::string currentSymbol = this->currentTape.getCurrentSymbol();

	// If there is still a transition left then we arent finished
	for (auto& trans : this->transitions) {
		if (trans.from.name == this->currentState.name &&
		    trans.read == currentSymbol) {
			return false;
		}
	}

	// No transition found, we finished in an error state!
	return true;
}

Transition TuringMachine::getNextTransition () {
	if (this->hasFinished()) return Transition();

	// Get the current symbol
	std::string currentSymbol = this->currentTape.getCurrentSymbol();

	// Get the transition
	for (auto& trans : this->transitions) {
		if (trans.from.name == this->currentState.name &&
		    trans.read == currentSymbol) {
			return trans;
		}
	}

	return Transition();
}

bool TuringMachine::accepted () {
	for (auto& state : this->finals) {
		if (state.name == this->currentState.name) return true;
	}

	return false;
}

void TuringMachine::nextStep () {
	if (this->hasFinished()) return;
	
	Transition currentTrans = this->getNextTransition();

	// Write the symbol for the transition
	if (currentTrans.write != std::string(""))
		this->currentTape.setCurrentSymbol(currentTrans.write);

	// Move the head
	if (currentTrans.headMove == "R") {
		this->currentTape.moveRight();
	} else if (currentTrans.headMove == "L") {
		this->currentTape.moveLeft();
	}

	// Go to the next state
	this->currentState = currentTrans.to;
}

void TuringMachine::run () {
	while (!this->hasFinished()) this->nextStep();
}