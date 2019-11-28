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
		std::vector<Token>elements;

	public:
		Var(int t, std::string n){type=t; name=n;}
		void Settype(int t){type = t;}
		void Setvalue(std::string v){value = v;}
		void Setname(std::string v){name = v;}
		int Gettype(){return type;}
		std::string Getvalue(){return value;}
		std::string Getname(){return name;}
		Token Getelement(int e){return elements[e];}


		void append(Token t){elements.push_back(t);}
		void setelements(std::vector<Token>e){elements = e;}
		std::vector<Token> getelements(){return elements;}
		void printelements(){
			for (Token t : elements)
				std::cout<<t.Getvalue()<<std::endl;
		}
};

Parse_state state;
std::vector<Var>variables;

void Expression();
void PrintVars();

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
	Expression();
	while(Accept(COMMA)){
		Expression();
	}
	Expect(CBRACKET);
}

void List(){
	ListExpression();
	Accept(CBRACKET);
}

int Getvar(std::string name){
	if(variables.size()==0)return -1;
	int x=0;
	for(Var v : variables){
		if(v.Getname() == name && v.Gettype()!=IDENT){
			return x;
		}
		x++;
	}
	return -1;
}

void PrintLine(int error){
	if(error == 0)std::cout<<"#Error"<<std::endl;
	while(!Accept(ENDLINE,true)){
		state.current_token++;
		state.line+=state.tokens[state.current_token].Getvalue();
	}
}

void Factor(char op = ' '){
	int index;
	Var t = variables.back();
	if(Getvar(t.Getname())==-1){
		index = variables.size()-1;
	}
	else{
		index = Getvar(t.Getname());
		Var t = variables[index];
	}

	if(Accept(NUMBER, true)){
		std::string value = state.tokens[state.current_token].Getvalue();
		if(op =='+'){
			value=std::to_string(stoi(value)+stoi(variables[index].Getvalue()));
		}
		if(op =='-'){
			value=std::to_string(stoi(variables[index].Getvalue())-stoi(value));
		}

		if(t.Gettype()!=LIST){
			t.Setvalue(value);
			t.Settype(NUMBER);
			variables[index] = t;
		}else{
			t.append(state.tokens[state.current_token]);
		}
			Accept(NUMBER);
			return;
	}
	if(Accept(IDENT,true)){
		int varindex;
		if((varindex=Getvar(state.tokens[state.current_token].Getvalue()))==-1)
			PrintLine(0);

		std::string value = variables[varindex].Getvalue();
		if(variables[index].Gettype()==NUMBER){
			if(op =='+'){
				value=std::to_string(stoi(value)+stoi(variables[index].Getvalue()));
			}
			if(op =='-'){
				value=std::to_string(stoi(variables[index].Getvalue())-stoi(value));
			}
		}else if(variables[index].Gettype()==STRING){
			if(op =='+'){
				if(variables[varindex].Gettype()==STRING){
					std::string value2=variables[index].Getvalue();
					value2.pop_back();
					value.replace(0,1,"");
					value=value2+value;
				}
				else {
					PrintLine(0);
					return;
				}
			}
		}else if(variables[index].Gettype()==LIST){
			std::vector<Token> original = variables[varindex].getelements();
			variables[index].printelements();
			for (Token t:original)std::cout<<t.Getvalue();
			std::vector<Token> added = variables[varindex].getelements();
			original.insert(original.end(), added.begin(), added.end());
			variables[index].setelements(original);
			variables[index].printelements();
		}

		t.Setvalue(value);
		t.Settype(variables[varindex].Gettype());
		variables[index] = t;
		Accept(IDENT);
	}
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
		t.Settype(LIST);
		t.Setvalue("");
		t.Settype(LIST);
		variables[index] = t;
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

	//for (auto i:tokens)
		//std::cout<<i.Getvalue()<<" " << Symbols[i.Gettype()]<<std::endl;
	//std::cout <<"\n--------------------------------------------"<<std::endl;
	while (state.current_token < state.tokens.size()){
		state.line = "";
		Statement();
		if(state.line == "")break;
		//std::cout<<state.line<<std::endl;
	}
	PrintVars();
}
