#include "parser.h"
#include <iostream>
#include <string>
#include <vector>

struct Parse_state {
	std::string line ="";
	int current_token =0;
	std::vector<Token> tokens;
};

int Accept(int type, Parse_state &state, bool peek = false){
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

void Factor(Parse_state &state){
	if(Accept(NUMBER, state)){return;}
	//if(Accept(STRING, state)){std::cout<<"STRING"<<std::endl;return;}
	if(Accept(NONE, state)){return;}
}

void Expression(Parse_state &state){
	if(Accept(OBRACKET, state)){
		Expression(state);
		if(Accept(CBRACKET,state)){return;}
	}
	Factor(state);
	while(Accept(MULT,state)||Accept(DIV,state))
		Factor(state);
}

void Assignment (Parse_state &state){
	Expression(state);
}

void Ident(Parse_state &state){
	if(Accept(EQUALS, state))Assignment(state);
}

void Statement(Parse_state &state){
	if(Accept(IDENT, state))
		Ident(state);
	Accept(ENDLINE, state);
	return;
}

void Parse(std::vector<Token>&tokens){
	Parse_state state;
	state.tokens = tokens;

	//for (auto i:tokens)
		//std::cout<<i.Getvalue()<<" " << Symbols[i.Gettype()]<<std::endl;
	while (state.current_token < state.tokens.size()){
		state.line = "";
		Statement(state);
		std::cout<<state.line<<std::endl;
	}
}
