#ifndef PARSETREE_H
#define PARSETREE_H

#include <vector>
#include <string>

// PARSETREE.H
//--------------------------------------------------------------------------

class ParseTree;

class ParseTreeNode {
  public:
  	std::string value;
  	std::vector<std::string> name;
  	std::vector<ParseTreeNode*> children;
  	bool isLeaf;
  	ParseTree* tree;
    int leafPosition;

    ParseTreeNode(std::string v, std::string n, ParseTree* t);
  	ParseTreeNode(std::string v, std::string n, ParseTree* t, int p);
  	void addChildren(std::string s, std::string nextDerivation, ParseTree* t);
};

//--------------------------------------------------------------------------

class ParseTree {
  public:
  	ParseTreeNode* root;
  	std::vector<ParseTreeNode*> leafNodes;

  	ParseTree(std::vector<std::string>& d, std::vector<std::string>& r);
  	ParseTreeNode* findLeftMostCapital();
  	void checkLeafNodes(std::string s);
};

#endif