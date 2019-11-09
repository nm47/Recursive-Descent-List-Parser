#include "parser.h"
#include <regex>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

Token match(std::string exp){
	std::vector<Token> candidates;
	std::smatch m;
	
	std::regex number("^[0-9]+");
	std::regex ident("^[a-zA-Z_][a-zA-Z0-9_]*");

	if(regex_search(exp,m, number)){candidates.push_back(Token(NUMBER, m[0]));}
	if(regex_search(exp,m, ident)){candidates.push_back(Token(IDENT, m[0]));}

	Token record;
	if(candidates.size() > 0){
		for (auto c : candidates)
			if(c.Getvalue().size() > record.Getvalue().size())
				record = c;
		return record;
	}

	if(exp.at(0)=='+'){return Token(PLUS,"+");}
	if(exp.at(0)=='['){return Token(OBRACKET,"[");}
	if(exp.at(0)==']'){return Token(CBRACKET,"]");}
	if(exp.at(0)=='='){return Token(EQUALS,"=");}
	if(exp.at(0)==' '){return Token(SPACE," ");}

	return Token();
}

void lex (std::string line, std::vector<Token>&tokens){
	std::string remaining = line;

	Token m;

	while(remaining.size() > 0) {
		m = match(remaining);
		remaining.erase(0, m.Getvalue().size());
		if(m.Gettype()==NONE){
			std::cout<<"nothing matched.."<<m.Getvalue()<<std::endl;
			break;
		}
		tokens.push_back(m);
	}
	tokens.push_back(Token(ENDLINE, "\0"));
}

void Read_to_lexer(std::string inputfile, std::vector<Token>&tokens){
	std::string line;
	std::ifstream file(inputfile);

	while(std::getline(file,line)){
		lex(line, tokens);
	}
	file.close();
}

int main (int argc, char *argv[]){
	if(argc <= 1){std::cout<<"not enough arguments"<<std::endl; return 1;}

	std::vector<Token> tokens;
	std::string input = argv[1];
	std::string file = input.substr(input.find('=')+1, input.size());
	Read_to_lexer(file, tokens);

	Parse(tokens);
	return 0;
}
