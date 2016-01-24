#include "TMGrapher.h"
#include <string>

TMGrapher::TMGrapher () {
	this->tm = TuringMachine();
}

TMGrapher::TMGrapher (TuringMachine& tm) {
	this->tm = tm;
}

// TM should not have ran yet
std::string TMGrapher::toDot () {
	std::string output = "digraph tm {\n";
	output += std::string("rankdir=\"LR\";\n");
	output += std::string("HIDDENSTARTSTATE [color=white, fontcolor=white];\n");
	output += std::string("HIDDENSTARTSTATE -> ") + this->tm.currentState.name + " [label=\"start\"];\n";

	for (auto& trans : this->tm.transitions) {
		std::string direction = trans.headMove == "R" ? "->" : "<-";
		output += trans.from.name + " -> " + trans.to.name + " [label=\"" + trans.read + " / " + trans.write + " " + direction + "\"];\n";
	}

	output += "}";

	return output;
}