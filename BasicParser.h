#ifndef BASIC_PARSER_H_
#define BASIC_PARSER_H_
#include"ASTstructures.h"
#include"Lexer.h"
#include<map>



class BasicParser
{

	class Precedence {
		int value;
		bool leftAssoc;
	public:
		Precedence(int v, bool l) {
			value = v;
			leftAssoc = l;
		}
		int getValue() {
			return value;
		}
		bool getLeftAssoc() {
			return leftAssoc;
		}
	};

public:
	BasicParser(const char* fileName) :lexer(fileName) {
		operators.insert(std::make_pair(std::string{ "<" }, new Precedence(1, true)));
        operators.insert(std::make_pair(std::string{ "<=" }, new Precedence(1, true)));
		operators.insert(std::make_pair(std::string{ ">" }, new Precedence(1, true)));
        operators.insert(std::make_pair(std::string{ ">=" }, new Precedence(1, true)));
		operators.insert(std::make_pair(std::string{ "+" }, new Precedence(2, true)));
		operators.insert(std::make_pair(std::string{ "-" }, new Precedence(2, true)));
		operators.insert(std::make_pair(std::string{ "*" }, new Precedence(3, true)));
		operators.insert(std::make_pair(std::string{ "/" }, new Precedence(3, true)));
		operators.insert(std::make_pair(std::string{ "==" }, new Precedence(2, true)));
        operators.insert(std::make_pair(std::string{ "!=" }, new Precedence(2, true)));
        operators.insert(std::make_pair(std::string{ "=" }, new Precedence(1, false)));
        operators.insert(std::make_pair(std::string{ "%" }, new Precedence(4, true)));
        operators.insert(std::make_pair(std::string{ "&&" }, new Precedence(6, true)));
        operators.insert(std::make_pair(std::string{ "||" }, new Precedence(6, true)));
        operators.insert(std::make_pair(std::string{ "+=" }, new Precedence(8, true)));
        operators.insert(std::make_pair(std::string{ "-=" }, new Precedence(8, true)));
        operators.insert(std::make_pair(std::string{ "*=" }, new Precedence(8, true)));
        operators.insert(std::make_pair(std::string{ "/=" }, new Precedence(8, true)));
        operators.insert(std::make_pair(std::string{ "&=" }, new Precedence(8, true)));
	}

	void token(std::string name);
    bool isToken(std::string name);
	ASTree* primary();
	ASTree* factor();
	ASTree* expression();
	ASTree* block();
	ASTree* statement();
	ASTree* program();
    ASTree* param();
    ASTree* params();
    ASTree* param_list();
    ASTree* def();
    ASTree* args();
    ASTree* postfix();
    ASTree* member();
    ASTree* class_body();
    ASTree* defclass();
    ASTree* createInstance();
    ASTree* elements();
    bool isEndOfFile() {
        if(lexer.peek()==&Token::EOF_TOKEN)
            return true;
        return false;
    }
private:
	Lexer lexer;
	
	ASTree* doShift(ASTree* left, int prec);
	Precedence* nextOperator();
	bool rightIsExpr(int prec, Precedence* nextPrec);
    void skipEOL() {
        while(isToken(Token::EOL)) {
            token(Token::EOL);
        }
    }
protected:
	std::map<std::string, Precedence*> operators;
};


#endif // !BASIC_PARSER_H_

