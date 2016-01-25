#include "ParseTree.h"
#include <iostream>
#include <sstream>
#include <ctype.h>
#include <stdio.h>

// PARSETREE.CPP
//--------------------------------------------------------------------------

std::string charToStr(char c) {
	std::stringstream ssm;
	std::string str;

	ssm << c;
	ssm >> str;

	return str;
}

//--------------------------------------------------------------------------

bool isCapital(std::string s) {
	if (s.length() > 1) {
		return false;
	}

	std::stringstream ssm;
	char c;

	ssm << s.c_str();
	ssm >> c;

	return isupper(c);
}

//--------------------------------------------------------------------------

bool containsCapitals(std::string s) {
	for (int i = 0; i < s.length(); ++i) {
		if (isCapital(charToStr(s[i])))
			return true;
	}

	return false;
}

//--------------------------------------------------------------------------

std::string removePrevious(std::string nextDerivation, std::string type) {
	std::string newStr = nextDerivation;

	// Delete all things that come before the type
	while (newStr.find(type) != std::string::npos) {
		newStr.erase(newStr.begin(), newStr.begin() + nextDerivation.find(type) + 3);
	}

	// Delete everything after the type
	newStr.erase(newStr.begin() + newStr.find(")") - 1, newStr.end());

	return newStr;
}

//--------------------------------------------------------------------------

std::string parseList(std::string list) {
	std::string newStr = "";

	while(list.find("\"") != std::string::npos) {
		// Erase everything before and including the first " (aka the opening ")
		list.erase(list.begin(), list.begin() + list.find("\"") + 1);

		// Add the next character to the newStr
		newStr += list[0];

		// Erase everything before the next " (aka the closing ")
		list.erase(list.begin(), list.begin() + list.find("\"") + 1);
	}

	return newStr;
}

//--------------------------------------------------------------------------

std::string parseNodeName(std::string nextDerivation, std::string type) {
	if (type == "c" || type == "d", type == "z") {
		return removePrevious(nextDerivation, type);
	}
	else if (type == "l") {
		std::string list = removePrevious(nextDerivation, type);
		return parseList(list);
	}
	else {
		return type;
	}
}

//--------------------------------------------------------------------------

ParseTreeNode::ParseTreeNode(std::string v, std::string n, ParseTree* t) {
	value = v;

	if (v == "l") {
		for (int i = 0; i < n.size(); ++i) {
			name.push_back(charToStr(n[i]));
		}
	}
	else {
		name.push_back(n);
	}

	isLeaf = true;
	tree = t;
}

//--------------------------------------------------------------------------

void ParseTreeNode::addChildren(std::string s, std::string nextDerivation, ParseTree* t) {
	std::string current;

	for (int i = 0; i < s.length(); ++i) {
		current = s[i];

		// Parse name for the new node
		std::string nodeName = parseNodeName(nextDerivation, current);

		ParseTreeNode* newNode = new ParseTreeNode(current, nodeName, t);
		children.push_back(newNode);
		tree->leafNodes.push_back(newNode);
	}

	isLeaf = false;

	// Remove leafNodes that are no longer leaves
	tree->checkLeafNodes(s);
}

//--------------------------------------------------------------------------

ParseTree::ParseTree(std::vector<std::string>& d, std::vector<std::string>& r) {
	// Create root
	root = new ParseTreeNode(d.front(), d.front(), this);
	leafNodes.push_back(root);

	ParseTreeNode* currentNode = root;

	// Add children for every derivation
	for (int i = 0; i < d.size(); ++i) {
		if (! containsCapitals(d[i])) {
			break;
		}

		currentNode = this->findLeftMostCapital();

		// Add the new children for the step of the derivation
		currentNode->addChildren(r[i], d[i + 1], this);
		currentNode = currentNode->children[0];
	}
}

ParseTreeNode* ParseTree::findLeftMostCapital() {
	ParseTreeNode* currentNode = root;

	for (int i = 0; i < leafNodes.size(); i++) {
		if (currentNode->isLeaf && isCapital(currentNode->value)) {
			break;
		}

		currentNode = leafNodes[i];
	}

	return currentNode;
}

void ParseTree::checkLeafNodes(std::string s) {
	// Remove nodes that are no longer leaves
	for (int i = 0; i < leafNodes.size(); ++i) {
		if (! leafNodes[i]->isLeaf) {
			leafNodes.erase(leafNodes.begin() + i);
			i--;
		}
	}
}