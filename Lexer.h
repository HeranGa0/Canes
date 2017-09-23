#ifndef LEXER_CPP
#define LEXER_CPP
#include <regex>
#include <iostream>
#include <string>
#include"Token.h"
#include<list>
#include<fstream>

class Lexer
{
public:
	
	Lexer(const char* fileName)
	{
		reader.open(fileName);
	}
	
	~Lexer() 
	{
		reader.close();
	}
	Token* peek(unsigned int i = 0) {
		if (fillQueue(i))
			return queue.at(i);
		else
			return &Token::EOF_TOKEN;
	}
	Token* read()
	{
		if (fillQueue(0)) {
			auto temp = *(queue.begin());
			queue.erase(queue.begin());
			return temp;
		}
		else
			return &Token::EOF_TOKEN;
	}

protected:
	void readLine();
	std::string addToken(std::string current);
	std::string toStringLiteral(std::string s);
private:
	

	unsigned int lineNumber = 0;
	//const std::regex spaceN{ std::string("\\s*") };
	const std::regex comment{ std::string("\\s*(//.*)") };
	const std::regex num{ std::string("\\s*[0-9]+") };
    const std::regex dnum{ std::string("\\s*[0-9]+\\.{1}[0-9]+") };
	const std::regex id{ std::string("\\s*([A-Z_a-z][A-Z_a-z0-9]*|==|!=|<=|>=|&&|\\|\\||[[:punct:]])") };
	const std::regex str{ std::string("\\s*\"(\\\\\"|\\\\\\\\|\\\\n|[^\"])*\"") };

	std::vector<Token*> queue;
	bool hasMore = true;
	std::ifstream reader;

	bool fillQueue(unsigned int i) {
		while (i >= queue.size()) {
			if (hasMore) {
				readLine();
			}

			else {
				return false;
			}
		}
		return true;
	}
    std::string trimming(std::string orin) {
        return orin.substr(orin.find_first_not_of(" "));
    }

};

#endif // !LEXER_CPP

