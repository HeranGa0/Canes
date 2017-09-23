#ifndef TOKEN_H_
#define TOKEN_H_
#include<string>

class Token
{
private:
	int lineNumber;
public:
	static const std::string EOL;
	static Token EOF_TOKEN;
	virtual int getLineNumber() {
		return lineNumber;
	}

	virtual bool isIdentifier() {
		return false;
	}

	virtual bool isNumber() {
		return false;
	}

	virtual bool isString() {
		return false;
	}
    virtual bool isDnumber() {
        return false;
    }

	virtual int getNumber() {
		return -1;
	}
    virtual double getDnumber() {
        return 0.1;
    }
	virtual std::string getText() {
		return "error";
	}

protected:
	 Token(int line) {
		 lineNumber = line;
	}
	

};

class NumToken :public Token
{
public:
	NumToken(int line, int v):Token(line) {
		value = v;
	}

	virtual bool isNumber() {
		return true;
	}

	virtual std::string getText() {
		return std::to_string(value);
	}

	virtual int getNumber() {
		return value;
	}
private:
	int value;
};

class DnumToken :public Token
{
public:
    DnumToken(int line, double v):Token(line) {
        value = v;
    }
    
    virtual bool isDnumber() {
        return true;
    }
    
    virtual std::string getText() {
        return std::to_string(value);
    }
    
    virtual double getDnumber() {
        return value;
    }
private:
    double value;
};


class IdToken :public Token {
public:
	IdToken(int line, std::string t) :Token(line) {
		text = t;
	}
	virtual bool isIdentifier() { return true; }
	virtual  std::string getText() { return text; }
	virtual int getNumber() {
		return -1;
	}
private:
	std::string text;
};


class StrToken :public Token {
public:
	StrToken(int line, std::string l) :Token(line) {
		literal = l;
	}
	virtual bool isString() { return true; }
	virtual  std::string getText() { return literal; }
	virtual int getNumber() {
		return -2;
	}
private:
	std::string literal;
};


#endif // !TOKEN_H_

