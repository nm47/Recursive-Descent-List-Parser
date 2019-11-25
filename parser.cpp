#include "parser.h"
#include <iostream>
#include <string>
#include <vector>

struct Parse_state {
	std::string line ="";
	int current_token =0;
	std::vector<Token> tokens;
};

class Var {
	private:
		int type = NONE;
		std::string name;
		std::string value = "";

	public:
		Var(int t, std::string n){type=t; name=n;}
		void Settype(int t){type = t;}
		void Setvalue(std::string v){value = v;}
		void Setname(std::string v){name = v;}
		int Gettype(){return type;}
		std::string Getvalue(){return value;}
		std::string Getname(){return name;}
};

Parse_state state;
std::vector<Var>variables;

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
	if(!Accept(type)){
		std::cout<<"Parsing Error: ";
		std::cout<<"Expected " <<Symbols[type]<<" Got: "<<Symbols[state.tokens[state.current_token].Gettype()]<<std::endl;
		std::cout<<state.line<<std::endl;
	}
}

void ListExpression(){
	std::cout<<state.line<<std::endl;
	Expression();
	std::cout<<"Expression Terminated"<<std::endl;
	std::cout<<state.line<<std::endl;
	while(Accept(COMMA)){
		Expression();
		//Add value to list vector
	}
	Expect(CBRACKET);
}

void List(){
	ListExpression();
	while(Accept(PLUS)){
		Expect(OBRACKET);
		ListExpression();
	}
	Accept(CBRACKET);
}

void Factor(char op = ' '){
	Var t = variables.back();
	if(Accept(NUMBER, true)){
		std::string value = state.tokens[state.current_token].Getvalue();
		if(op =='+')
			value=std::to_string(stoi(value)+stoi(variables.back().Getvalue()));
		if(op =='-')
			value=std::to_string(stoi(variables.back().Getvalue())-stoi(value));

		t.Setvalue(value);
		t.Settype(NUMBER);
		variables.back() = t;
		Accept(NUMBER);
		return;
	}
	if(Accept(IDENT)){std::cout<<"IDENT"<<std::endl;return;}
	if(Accept(STRING,true)){
		std::string value = state.tokens[state.current_token].Getvalue();
		t.Setvalue(value);
		if(op=='+')
			value=value+variables.back().Getvalue();
		//if(op=='-')//ERROR
		t.Settype(STRING);
		variables.back() = t;
		Accept(STRING);
		return;
	}
	if(Accept(OBRACKET)){
		variables.back().Settype(LIST);
		t.Setvalue("[]");
		t.Settype(LIST);
		variables.back() = t;
		List();
	}
}

void Term(char op = ' '){
	Factor(op);
	while(Accept(MULT)||Accept(DIV))
		Factor();
}

void Expression(){
	Accept(LPAREN);
	Term();
	while(Accept(PLUS)||Accept(MINUS)){
		char op = state.line.back();
		Term(op);
	}
	Accept(RPAREN);
}

void Assignment(){
	Expression();
}

void Ident(){
	Token t = state.tokens[state.current_token];
	Var v = Var(t.Gettype(), t.Getvalue()); 
	variables.push_back(v);
	
	Accept(IDENT);
	if(Accept(EQUALS))
		Assignment();
}

void Print(){
	Accept(LPAREN);
	Expression();
	Accept(RPAREN);
}

void Statement(){
	if(Accept(IDENT,true))
		Ident();
	if(Accept(PRINT))
		Print();


	Expect(ENDLINE);
	return;
}

void PrintVars(){

	std::cout <<"\n--------------------------------------------"<<std::endl;
	std::cout<<"Variables"<<std::endl;
	for (auto i:variables)
		std::cout<<i.Getname() << " = "<<  i.Getvalue()<<" " << Symbols[i.Gettype()]<<std::endl;

}

void Parse(std::vector<Token>&tokens){
	state.tokens = tokens;

	for (auto i:tokens)
		std::cout<<i.Getvalue()<<" " << Symbols[i.Gettype()]<<std::endl;
	std::cout <<"\n--------------------------------------------"<<std::endl;
	while (state.current_token < state.tokens.size()){
		state.line = "";
		Statement();
		if(state.line == "")break;
		//std::cout<<state.line<<std::endl;
	}
	PrintVars();
}
