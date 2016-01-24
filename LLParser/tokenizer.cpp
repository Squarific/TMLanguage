#ifndef TOKENIZER_CPP
#define TOKENIZER_CPP
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <streambuf>
#include <algorithm>
#include <cctype>
#include "tokenizer.h"

using namespace std;

    token::token(string a, string s){
        abstract = a;
        specific = s;
    }
    token::token(const token& other){
        abstract = other.abstract;
        specific = other.specific;
    }
    token& token::operator=(const token& other){
        abstract = other.abstract;
        specific = other.specific;
    }
	string token::print(){
		string out;
		out +=this->abstract;
		if(this->specific!=""){
			out +="(";
			out +=this->specific;
			out +=")";
		}
		return out;
	}
    ostream& operator<<(ostream& os, const token& t){
		os<<t.abstract;
		if(t.specific!=""){
			os<<"("<<t.specific<<")";
		}
		return os;
	}

vector<string> wordsToVector(string filename){
// file to string
	ifstream ifs(filename);
	string content( (istreambuf_iterator<char>(ifs) ),(istreambuf_iterator<char>()));
//	cout<<content<<endl;
//woord per woord op stack zetten
	vector<string> woorden;
    istringstream iss(content);
    string word;
    while(iss >> word) {
      //  cout<<word<<endl;
		woorden.push_back(word);
    }
//verwijdert whitespace
//    content.erase(remove_if(content.begin(),content.end(),[](char x){return isspace(x);}), content.end());
//    cout << content << '\n';
	return woorden;
}

bool listSyntaxCheck(string x){return true;}
bool nameSyntaxCheck(string x){return true;}

bool tokenizer(vector<token> & Tokens, string inputfile){

    vector<string> codeVector = wordsToVector(inputfile);

    while(!codeVector.empty()){
        string temp = codeVector.front();
        if(temp == "IF"){
            token tok("i");
            Tokens.push_back(tok);
        }else if(temp == "WHILE"){
            token tok("w");
            Tokens.push_back(tok);
        }else if(temp == "END"){
            token tok("e");
            Tokens.push_back(tok);
        }else if(temp == "MOVELEFT"){
            token tok("m");
            Tokens.push_back(tok);
        }else if(temp == "MOVERIGHT"){
            token tok("r");
            Tokens.push_back(tok);
        }else if(temp == "ACCEPT"){
            token tok("a");
            Tokens.push_back(tok);
        }else if(temp == "REJECT"){
            token tok("n");
            Tokens.push_back(tok);
        }else if(temp.front() == '[' and temp.back() == ']' ){
            if(listSyntaxCheck(temp)){
                token tok("l", temp);
                Tokens.push_back(tok);
            }else{cout<<"list syntax error: "<< temp <<endl;}
        }else if(temp.front() == '_' and temp.back() == '_' ){
            if(nameSyntaxCheck(temp)){
                token tok("c", temp);
                Tokens.push_back(tok);
            }else{cout<<"functionname syntax error: "<< temp <<endl;}
        }else if(temp.front() == '*' and temp.back() == '*' ){
            if(nameSyntaxCheck(temp)){
                token tok("d", temp);
                Tokens.push_back(tok);
            }else{cout<<"functionname syntax error: "<< temp <<endl;}
        }else{
            cout<<"tokenizer did not recognize: " << temp<<endl;
            return false;
        }
        codeVector.erase (codeVector.begin());
    }
    token tok("$"); //end of string
    Tokens.push_back(tok);
//    for(auto i: Tokens){cout<<i;}
    return true;
}
#endif // TOKENIZER_CPP
