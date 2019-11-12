#include "parser.h"
#include <iostream>
#include <string>
#include <vector>

struct Parse_state {
	std::string line ="";
	int current_token =0;
	std::vector<Token> tokens;
};

Parse_state state;
void Expression();

int Accept(int type, bool peek = false){
	while(state.tokens[state.current_token].Gettype()==SPACE){
		state.line+=' ';
		state.current_token++;
	}	
	if(state.tokens[state.current_token].Gettype()==type){
		if(!peek){
			state.line+=state.tokens[state.current_token].Getvalue();
			state.current_token++;
		}
		return 1;
	}
	return 0;
}

void Expect(int type){
	if(!Accept(type))
		std::cout<<"Parsing Error"<<std::endl;
}

void List(){
	Expression();
	Expect(CBRACKET);
}

void Factor(){
	if(Accept(NUMBER))
		return;
	if(Accept(IDENT))return;
	if(Accept(STRING))return;
	if(Accept(OBRACKET))
		List();
}

void Term(){
	Factor();
	while(Accept(MULT)||Accept(DIV))
		Factor();
}

void Expression(){
	Accept(LPAREN);
	Term();
	while(Accept(PLUS)||Accept(MINUS)){
		Term();
	}
	Accept(RPAREN);
}

void Assignment (){
	Expression();
}

void Ident(){
	if(Accept(EQUALS))
		Assignment();
}

void Print(){
	Accept(LPAREN);
	Expression();
	Accept(RPAREN);
}

void Statement(){
	if(Accept(IDENT))
		Ident();
	if(Accept(PRINT))
		Print();

	Expect(ENDLINE);
	return;
}

void Parse(std::vector<Token>&tokens){
	state.tokens = tokens;

	//for (auto i:tokens)
		//std::cout<<i.Getvalue()<<" " << Symbols[i.Gettype()]<<std::endl;
	while (state.current_token < state.tokens.size()){
		state.line = "";
		Statement();
		if(state.line == "")return;
		std::cout<<state.line<<std::endl;
	}
}
