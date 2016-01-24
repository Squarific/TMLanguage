#ifndef TOKENIZER_H
#define TOKENIZER_H
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <streambuf>
#include <algorithm>
#include <cctype>

using namespace std;

class token{
    public:

    string abstract;//abstract
    string specific;//specfieke code als nodig, enkel gebruikt bij list en functienaam

    token(string a, string s = "");
    token(const token& other);
    token& operator=(const token& other);
	string print();
	friend ostream& operator<<(ostream& os, const token& t);	
};

vector<string> wordsToVector(string filename);

bool listSyntaxCheck(string x);
bool nameSyntaxCheck(string x);

bool tokenizer(vector<token> & Tokens, string inputfile = "program.txt");
#endif // TOKENIZER_H
