#ifndef TuringMachine_H
#define TuringMachine_H

#include <vector>
#include <string>

class State {
public:
	std::string name;
	State() {};
	State(std::string name);
};

class Transition {
public:
	State from;
	State to;

	std::string headMove;
	std::string read;
	std::string write;

	Transition () {};
	Transition(State from, State to, std::string headMove, std::string read, std::string write);
};

class Tape {
private:
	// Current configuration
	// This std::vector should be used as a tape
	// where 0 is the center, 1 is to the right and 2 is to the left
	// 3 is right of 1 and 4 is left of 2
	// eg: ...6420135...
	std::vector<std::string> tape = {};
	int head = 0;

public:
	std::string blank;

	Tape () {};
	Tape(std::string blank);

	// Creates a tape where the std::string will be started at 0 going to the right
	Tape(std::string blank, std::string startString);

	// Get the symbol under the head
	std::string getCurrentSymbol();

	// Put the given symbol on the tape under the head
	void setCurrentSymbol(std::string symbol);

	// Move the head
	void moveLeft();
	void moveRight();
};

class TuringMachine {
public:
	// Static state information
	std::vector<State> states;
	std::vector<State> finals;

	std::vector<Transition> transitions;

	// Static symbol information
	std::vector<std::string> inputSymbols;
	std::vector<std::string> tapeSymbols;
	std::string blank;

	// Current configuration
	State currentState;
	Tape currentTape;

	TuringMachine() {}
	TuringMachine(std::vector<State> states,
	              std::vector<std::string> inputSymbols,
	              std::vector<std::string> tapeSymbols,
	              std::vector<Transition> transitions,
	              State start,
	              std::string blank,
	              std::vector<State> finals,
	              Tape tape);

	// Adds the machine with the given transition as a module
	// All final states will be removed and instead all transitions
	// going to the old final state will go to the given return state
	bool addModule (Transition trans, TuringMachine module, State& returnState);
	
	// Returns the next transition
	// If called when finished returns an empty Transition
	// If there is no Transition returns empty Transition
	Transition getNextTransition();

	// Returns true if the turing machine has nothing left to do
	bool hasFinished();

	// Returns true if the turing machine entered an accept state
	bool accepted();

	// Executes 1 step
	void nextStep();

	// Executes all steps
	void run();
};

#endif