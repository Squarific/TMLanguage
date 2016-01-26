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
#include "LLParser.h"

std::string epsilon = "y";
char epsilonChar = 'y';

bool setContains(std::set<char> s, char c){

	for(auto i : s){
		if(i==c){
			return true;
		}
	}
	return false;
}

bool capsCheck(char c){
    if(isupper(c)){
        return true;
    }else{
        return false;
    }	
}

char stringToChar(std::string in){
	return in.at(0);
}

std::string charToString(char a){

	std::stringstream ss;
	std::string s;
	ss << a;
	ss >> s;
	return s;

}

void readCfgFromXml(std::set<std::string> &terminals, std::set<std::string> &variables, std::string &startSymbol, std::vector<rule> &rules, std::string filename){
//epsilon bodies zijn gewoon "e" opgepast

	using namespace tinyxml2;

	XMLDocument xmlDoc;
//	std::cin>>filename;
	XMLError eResult = xmlDoc.LoadFile(filename.c_str());

	// volgende moet nog aangepast adhv hoe variabelen in inputfile zitten, wss nog loopen
	XMLNode * pRoot = xmlDoc.FirstChild();
	const char* x;

	XMLElement * pElement = pRoot->FirstChildElement("Variables");
	x = pElement->GetText();
	std::string varString=x;
	for(int i =0; i!=varString.length();i++){		
		variables.insert(charToString(varString[i]));
	}

	pElement = pElement->FirstChildElement("StartSymbol");
	x = pElement->GetText();
	startSymbol=x;

	pElement = pRoot->FirstChildElement("Terminals");
	x = pElement->GetText();
	std::string TerminalString=x;
	for(int i =0; i!=TerminalString.length();i++){		
		terminals.insert(charToString(TerminalString[i]));
	}

	XMLElement *pListElement = pRoot->FirstChildElement("Productions")->FirstChildElement("Rule");
	rule regel;
	while (pListElement != nullptr)
	{
		rule regel;
		std::string body;
		std::string head;
		x=pListElement->Attribute("LHS");
		head=x;
		x=pListElement->Attribute("RHS");
		body=x;
		std::string littleBody;
		regel.setHead(head);
		for(int i = 0; i!=body.length();i++){
			//std::cout<<body[i];
			if(body[i]!='|'){
				//std::cout<<'1';
				littleBody+=body[i];
			}
			if(body[i]=='|' or i==(body.length()-1)){
				//std::cout<<'2';
				regel.addBody(littleBody);
				littleBody.clear();
			}
		}
		rules.push_back(regel);
		pListElement = pListElement->NextSiblingElement("Rule");
	}

	//alles uitprinten 
//	std::cout<<"SS: "<<startSymbol<<std::endl;
//	for(auto i : terminals){std::cout<<"terminal: "<<i<<std::endl;}
//	for(auto i : variables){std::cout<<"variable: "<<i<<std::endl;}
//	for(auto i : rules){

//		std::cout<<"rule: "<< i.getHead()<<"->";
//		for(auto j: i.getBodies()){
//			std::cout<<j<<'|';
//		}
//		std::cout<<std::endl;

//	}

}
void calculateFirstSets(std::set<std::string> &variables,std::vector<rule> &rules, std::vector<std::set<char>> &firstSets){

	using namespace std;
	for (auto i :variables){
		set<char> x;
		firstSets.push_back(x);

	}
	int oldSize = 0;
	while(true){
		for (auto i : rules){
			string head = i.getHead();
			int position = 0;
			for(auto j : variables){
				if(j==head){
					break;
				}
				position++;
			}
			for(auto j : i.getBodies()){
				bool onlyVariables = true;
				bool allContainEpsilon = true;
				for(auto h: j){
					if(!capsCheck(h)){
						onlyVariables =	false;
					}
				}

				char currentChar = j.at(0);
				if(!capsCheck(currentChar)){

					firstSets.at(position).insert(currentChar);

				}else{
					int currentPos = 0;
					while(true){
						if(!capsCheck(currentChar)){

							firstSets.at(position).insert(currentChar);

							break;
						}else{
							int pos = 0;
							for(auto j : variables){
								if(j==charToString(currentChar)){
									break;
								}
								pos++;
							}
							//firstSets.at(position).insert(firstSets.at(pos).begin(),firstSets.at(pos).end());
							bool containsEpsilon = false;

							for(auto l: firstSets.at(pos)){

								if(l==epsilonChar){
									containsEpsilon = true;	
								}else{

									firstSets.at(position).insert(l);

								}
							}
							if(containsEpsilon){
								currentPos++;
								if(currentPos>j.size()-1){
									break;
								}
								currentChar=j.at(currentPos);
								continue;
							}else{
								allContainEpsilon = false;
								break;
							}
						}
					}
				}
				if(allContainEpsilon and onlyVariables){
					firstSets.at(position).insert(epsilonChar);
				}
			}	
		}
		int newSize = 0;
		for(auto i : firstSets){
				newSize+= i.size();
		}
		if(newSize>oldSize){
			oldSize = newSize;
		}else{
			break;
		}
	}
//	cout<<firstSets.size()<<endl;
//	for (auto i : firstSets){
//		for (auto j : i){
//			cout<< j<<',';

//		}
//	cout<<endl;
//	}
}

void calculateFollowSets(std::set<std::string> &variables,std::vector<rule> &rules, std::vector<std::set<char>> &firstSets, std::vector<std::set<char>> &followSets, std::string &startSymbol){

	using namespace std;
	for (auto i :variables){
		set<char> x;
		if(i==startSymbol){
			x.insert('$');
		}
		followSets.push_back(x);


	}
	int oldSize = 0;
	while(true){
		for(auto i : rules){
			for (auto j: i.getBodies()){
				for (int k = 0; k!= j.size()-1;++k){//-1 want op de laatste kan niets volgen
					if(capsCheck(j.at(k))){
						int pos = 0;
						for(auto u: variables){
							if(u==charToString(j.at(k))){
								break;
							}
							pos++;
						}
						if(!capsCheck(j.at(k+1))){//de volgende is een terminal
							followSets.at(pos).insert(j.at(k+1));
						}else{//de volgende is een variabel, voeg de firstset van die variabele toe, als y in de firstset ga door naar de volgende positie tot een terminal of variabele met firstset zonder y
							int currentPosition = 0;
							while(true){
								if(k+1+currentPosition>j.size()-1){
									break;
								}
								if(!capsCheck(j.at(k+1+currentPosition))){
									followSets.at(pos).insert(j.at(k+1+currentPosition));
									break;	
								}else{
									int posit = 0;
									for(auto u: variables){
										if(u==charToString(j.at(k+1+currentPosition))){
											break;
										}
										posit++;
									}
									bool anotherOne = false;
									for(auto n: firstSets.at(posit)){
										if(n==epsilonChar){
											anotherOne = true;			
										}else{
											followSets.at(pos).insert(n);
										}
									}
									if(anotherOne){
										currentPosition++;
										continue;
									}else{
										break;
									}
								}							
							}
						}
					}
				}
			}
		}
		int newSize = 0;
		for(auto i : followSets){
				newSize+= i.size();
		}
		if(newSize>oldSize){
			oldSize = newSize;
		}else{
			break;
		}
	}
//	cout<<followSets.size()<<endl;
//	for (auto i : followSets){
//		for (auto j : i){
//			cout<< j<<',';

//		}
//	cout<<endl;
//	}
}
std::set<char> firstSetOfBody(std::string body, std::set<std::string> &terminals, std::set<std::string> &variables, std::vector<std::set<char>> &firstSets){
	using namespace std;
	set<char> out;

	bool allVars = true;
	bool allContainEpsilon = true;
	for(auto i: body){
		if(!capsCheck(i)){
			allVars=false;
		}
	}

	int currentPos = 0;
	while(true){
		if(currentPos>body.size()-1){
			break;
		}
		char currentChar = body.at(currentPos);
		if(!capsCheck(currentChar)){
			out.insert(currentChar);
			break;
		}else{
			int posit = 0;
			for(auto u: variables){
				if(u==charToString(currentChar)){
					break;
				}
				posit++;
			}
			bool containsEpsilon = false;
			for(auto u: firstSets.at(posit)){
				if(u==epsilonChar){
					containsEpsilon = true;					
				}else{
					out.insert(u);				
				}
			}
			allContainEpsilon*=containsEpsilon;
			if(containsEpsilon){
				currentPos++;
				continue;
			}else{
				break;
			}
		}
	}
	if(allContainEpsilon and allVars){
		out.insert(epsilonChar);
	}
	return out;
}

std::vector<std::vector<std::string>> makeParseTable(std::set<std::string> &terminals, std::set<std::string> &variables, std::string &startSymbol, std::vector<rule> &rules){


	using namespace std;
	//sets in de vector in de volgorde van de variabelen in variables
	vector<set<char>> firstSets;
	vector<set<char>> followSets;
	calculateFirstSets(variables,rules, firstSets);
	calculateFollowSets(variables,rules, firstSets, followSets,startSymbol);
	int temp = terminals.size()+1;
	const int ts = temp;
	const int vs = variables.size();
	vector<vector<string>> parseTable;//var/term
	for(int i = 0;i!=vs;++i){
		vector<string> row;
		for(int j = 0; j!=ts;++j){
			row.push_back("leeg");
		}
		parseTable.push_back(row);
	}
//T[A,a] contains the rule A → w if and only if
//a is in Fi(w) or
//ε is in Fi(w) and a is in Fo(A).
	for(auto i: rules){
		int varPos =0;
		for(auto u : variables){
			if(u==i.getHead()){
				break;
			}
			varPos++;

		}
		for(auto j: i.getBodies()){

			set<char> firstSetje = firstSetOfBody(j, terminals, variables, firstSets );

			int termPos = 0;
			for(auto k: terminals){

				if(setContains(firstSetje, k.at(0))){
					if(parseTable.at(varPos).at(termPos)!="leeg"){
						cout<<"double entry game over";
						vector<vector<string>> x;
						return x ;
						}
					parseTable.at(varPos).at(termPos) = j;
				}else if(setContains(firstSetje, epsilonChar) and setContains(followSets.at(varPos), k.at(0))){
					if(parseTable.at(varPos).at(termPos)!="leeg"){
						cout<<"double entry game over";
						vector<vector<string>> x; 
						return x ;
						}
					parseTable.at(varPos).at(termPos) = j;

				}
			termPos++;
			}

		}

	}
//printing
//	cout<<"\t";
//	for(auto j:terminals){
//	cout<<j<<"\t";
//	}
//	cout<<"EOS";
//	cout<<endl;
//	int count = 0;
//	for(auto j: parseTable){
//		int incount = 0;
//		for(auto k: variables){if(count==incount)cout<<k<<"\t"; incount++;}

//	count++;
//		for(auto i: j){
//			cout<<i<<"\t";
//		}
//		cout<<endl;
//	}
	return parseTable;
}
bool parse(std::set<std::string> &terminals, std::set<std::string> &variables, std::vector<token> &inputString, std::stack<token> &Stack, std::vector<std::vector<std::string>> &parseTable, std::vector<std::string> &derivation, 	vector<string> &usedReplacements){

	using namespace std;

	vector<token> initialString = inputString;
	while(inputString.front().abstract!="$"){

		string topVar = Stack.top().abstract;
		string lookAhead = inputString.front().abstract;
		int varPos = 0;
		int termPos = 0;
		for(auto i: variables){if(i==topVar){break;}varPos++;}
		for(auto i: terminals){if(i==lookAhead){break;}termPos++;}

		string replacement = parseTable.at(varPos).at(termPos);


		if(replacement=="leeg"){cout<<"syntax error"<<endl;return false;}
		else if(replacement==epsilon){Stack.pop();usedReplacements.push_back(epsilon);}

		else{
			usedReplacements.push_back(replacement);
			Stack.pop();

			for(int i = replacement.size()-1; i!=-1;--i){
				Stack.push(charToString(replacement.at(i)));
			}
		}

		while(!capsCheck(Stack.top().abstract.at(0)) and !Stack.empty()){
			if(inputString.empty()){cout<<"inputstring empty prematurely"<<endl;}
			if(Stack.top().abstract == inputString.front().abstract){
				Stack.pop();
				inputString.erase(inputString.begin());
			}else{cout<<"stack-input ongelijkheid van terminals"<<endl;}
		}


		int u = initialString.size()-inputString.size();
		string step;
		stack<token> temp = Stack;

		for(int j = 0; j!= u; j++){
			step+=initialString.at(j).print();
		}

		while(!temp.empty()){
			step+=temp.top().print();
			temp.pop();
		}
		derivation.push_back(step);
	}
	if(capsCheck(derivation.back().back())){

		int varPos = 0;
		bool nullable = false;
		string x = charToString(derivation.back().back());
		for(auto i: variables){
			if(i==x){
				break;
			}
			varPos++;
		}

		for(auto i: parseTable.at(varPos)){
			if(i==epsilon){
				nullable=true;
			}
		}
		if(nullable){
			string newBack = derivation.back();
			newBack.erase(newBack.end()-1);
			derivation.push_back(newBack);
			usedReplacements.push_back(epsilon);
		}else{
			return false;
		}
	}
	return true;
}

bool CfgToPdaAndTest (std::string filename, std::string cfg, std::vector<std::string>& derivation, std::vector<std::string>& usedReplacements){
// cfg naar pda en test strings
	using namespace std;

	std::set<std::string> terminals;
	std::set<std::string> variables;
	std::string startSymbol;
	std::vector<rule> rules;

	readCfgFromXml(terminals,variables,startSymbol,rules, cfg);
	vector<vector<string>> parseTableStrings = makeParseTable(terminals,variables,startSymbol,rules);

	vector<token> inputString;

	bool tokenizeSucces = tokenizer(inputString,filename);//default argument white.txt het programma
	if(!tokenizeSucces){std::cout<<"tokenizer error"<<endl;return false;}
	
	stack<token> Stack;
	Stack.push(startSymbol);
	derivation.push_back(startSymbol);
	bool answer = parse(terminals,variables,inputString, Stack, parseTableStrings,derivation, usedReplacements);

	return answer;
}
