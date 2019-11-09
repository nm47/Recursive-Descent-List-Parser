#ifndef parser
#define parser

#include <vector>
#include <string>

typedef enum {NONE, SPACE, IDENT, NUMBER, ENDLINE, EQUALS, PLUS, MULT,DIV, OBRACKET, CBRACKET}Symbol;

class Token {
	private:
		int type;
		std::string value;
	public:
		Token(){ type = NONE; value = ""; }
		Token(int s, std::string v){ type = s; value = v; }
		std::string Getvalue() const{return value;}
		int Gettype() const{return type;}
};

void Parse(std::vector<Token>&tokens);

static char const * Symbols[] = 
{
	"NONE",
	"SPACE",
	"IDENT",
	"NUMBER",
	"ENDLINE",
	"EQUALS",
	"PLUS",
	"MULT",
	"DIV",
	"OBRACKET",
	"CBRACKET"
};
#endif
