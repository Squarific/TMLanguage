#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <tuple>
#include "tinyxml2/tinyxml2.h"
#include <stack>
#include <sstream>
#include <string>
#include <fstream>
#include "ctype.h"
#include "tokenizer.h"

bool setContains(std::set<char> s, char c);

bool capsCheck(char c);

char stringToChar(std::string in);

std::string charToString(char a);

class rule{
	public:
		rule(){}
		~rule(){}
		void setHead(std::string hoofd){
			this->head=hoofd;
		}
		void addBody(std::string lichaam){
			this->bodies.insert(lichaam);
		}
		std::string getHead(){
			return this->head;
		}
		std::set<std::string> getBodies(){
			return this->bodies;
		}
		rule(const rule& other){
			this->head=other.head;
			this->bodies=other.bodies;
		}
	private:
		std::string head;
		std::set<std::string> bodies;
};

void readCfgFromXml(std::set<std::string> &terminals, std::set<std::string> &variables, std::string &startSymbol, std::vector<rule> &rules, std::string filename);

void calculateFirstSets(std::set<std::string> &variables,std::vector<rule> &rules, std::vector<std::set<char>> &firstSets);

void calculateFollowSets(std::set<std::string> &variables,std::vector<rule> &rules, std::vector<std::set<char>> &firstSets, std::vector<std::set<char>> &followSets, std::string &startSymbol);

std::set<char> firstSetOfBody(std::string body, std::set<std::string> &terminals, std::set<std::string> &variables, std::vector<std::set<char>> &firstSets);

std::vector<std::vector<std::string>> makeParseTable(std::set<std::string> &terminals, std::set<std::string> &variables, std::string &startSymbol, std::vector<rule> &rules);

bool parse(std::set<std::string> &terminals, std::set<std::string> &variables, std::vector<token> &inputString, std::stack<token> &Stack, std::vector<std::vector<std::string>> &parseTable, std::vector<std::string> &derivation, 	vector<string> &usedReplacements);

bool CfgToPdaAndTest(std::string filename, std::string cfg, std::vector<std::string>& derivation, std::vector<std::string>& usedReplacements);