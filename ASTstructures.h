#ifndef AST_STRUCTURES_H_
#define AST_STRUCTURES_H_
#include<vector>
#include<algorithm>
#include<iostream>
#include"Token.h"
#include"Enviroment.h"
#include"Informations.h"

class ASTree;
using ASTiterator = std::vector<ASTree*>::iterator;


class ASTree
{
public:
	virtual ASTree* child(int i) = 0;
	virtual int numChildren() = 0;
	virtual ASTiterator children() = 0;
	virtual std::string location() = 0;
	virtual  ASTiterator iterator() {
		return children();
	}
	virtual std::string toString() = 0;
    virtual Object* eval(Enviroment&env)=0;
    
};

class ASTLeaf : public ASTree
{
private:
	static std::vector<ASTree*> empty;

protected:
	Token* _token=nullptr;
public:
	ASTLeaf(Token * t) : _token(t) {}
	ASTree* child(int i) {
		std::cout << "ASTLeaf has no child";
		return nullptr;
	}
	int numChildren() {
		return 0;
	}
	ASTiterator children() {
		return empty.begin();
	}
	std::string toString() {
		return _token->getText();
	}
	std::string location() {
        return "at line" + std::to_string(_token->getLineNumber()) ;
	}
	virtual Token* token() {
		return _token;
	}
    
    virtual Object* eval(Enviroment&env) {
        std::cerr<<__FILE__<<__LINE__<<std::endl;
        return nullptr;
    }
    
	virtual ~ASTLeaf()
	{
		delete _token;
	}
};

class ASTList : public ASTree
{
public:
	ASTList( const std::vector<ASTree*>& list):_children(list) {
		
	}
	virtual ASTree* child( int i) {
		return _children.at(i);
	}
	int numChildren() {
		return (int)_children.size();
	}
	ASTiterator children() {
		return _children.begin();
	}
    virtual Object* eval(Enviroment&env) {
        std::cerr<<__FILE__<<__LINE__<<std::endl;
        return nullptr;
    }
    
	std::string toString();

	std::string location();

protected:
	std::vector<ASTree*> _children;

};

class NumberLiteral : public ASTLeaf
{
public:
	NumberLiteral(Token * t):ASTLeaf(t) {

	}
	int value() {
		return token()->getNumber();
	}
    virtual Object* eval(Enviroment&env) {
        return new ObjectSub<int>(value());
    }
};

class DnumberLiteral : public ASTLeaf
{
public:
    DnumberLiteral(Token * t):ASTLeaf(t) {
        
    }
    double value() {
        return token()->getDnumber();
    }
    virtual Object* eval(Enviroment&env) {
        return new ObjectSub<double>(value());
    }
};

class Name :public ASTLeaf
{
public:
	Name(Token* t):ASTLeaf(t){}
	std::string name() {
		return token()->getText();
	}
    virtual Object* eval(Enviroment&env) {
        return(env.get(name()));
    }
};

class StringLiteral : public ASTLeaf
{
public:
	StringLiteral(Token* t) :ASTLeaf(t) {}
	std::string value() {
		return token()->getText();
	}
    virtual Object* eval(Enviroment&env) {
        return new ObjectSub<std::string>(value());
    }
};


class BinaryExpr :public ASTList
{
protected:
    Object* computeAssign(Enviroment& env,Object* rvalue);
    Object* computeINumber(int l, std::string op,int r);
    Object* computeDNumber(double l, std::string op,double r);
    

    Object* computeOp(Object* l , std::string op, Object* r);
public:
	BinaryExpr(const std::vector<ASTree*>& c):ASTList(c){}
	ASTree* left() {
		return child(0);
	}
	std::string oPerator() {
		return (static_cast<ASTLeaf*>(child(1)))->token()->getText();
	}
    ASTree* right() {
        return child(2);
    }
    virtual Object* eval(Enviroment& env);
};

class Postfix:public ASTList
{
public:
    Postfix(const std::vector<ASTree*> &c):ASTList(c){}
    virtual Object* eval(Enviroment& callerEnv,Object* value ) = 0;
};


class Arguments :public Postfix
{
public:
    Arguments(const std::vector<ASTree*> &c):Postfix(c){}
    int size() {
        return numChildren();
    }
    virtual Object* eval(Enviroment& callerEnv,Object* value ) override ;
};

class FixedArguments: public Arguments
{
public:
    FixedArguments(std::vector<ASTree*>& c):Arguments(c){}
    virtual Object* eval(Enviroment& callerEnv,Object* value ) override ;
};


class PrimaryExpr : public ASTList
{
private:
	PrimaryExpr(const std::vector<ASTree*>& c):ASTList(c) {}
public:
	static ASTree* create(const std::vector<ASTree*>&c) {
		return c.size() == 1 ? c.at(0) : new PrimaryExpr(c);
	}
    ASTree* operand() {
        return child(0);
    }
    Postfix* postfix(int nest) {
        return dynamic_cast<Postfix*>((child(numChildren()-nest-1)));
    }
    bool hasPostfix(int nest) {
        return (numChildren() - nest)>1;
    }
    Object* eval(Enviroment& env) {
        return evalSubExpr(env, 0);
    }
    Object* evalSubExpr(Enviroment& env, int nest) {
        if(hasPostfix(nest)) {
            auto target = evalSubExpr(env, nest+1);
            return postfix(nest)->eval(env, target);
        }
        else
            return operand()->eval(env);
    }
};

class NegativeExpr : public ASTList
{
public:
	NegativeExpr(const std::vector<ASTree*>&c) :ASTList(c) {

	}

	ASTree* operand() {
		return child(0);
	}

	std::string toString() {
		return "-" + operand()->toString();
	}
    virtual Object* eval(Enviroment env) {
        auto v = (operand())->eval(env);
        if(auto iv=dynamic_cast<ObjectSub<int>*>(v)) {
            iv->value = (-1)*(iv->value);
            return iv;
        }
        else {
            PRINT_ERR_ARGS("NegativeExpr should be IntType!")
            return nullptr;
        }
            
    }
};

class BlockStmnt : public ASTList
{
public:
	BlockStmnt(const std::vector<ASTree*>&c):ASTList(c) {}
    virtual Object* eval(Enviroment&env) {
        Object* result = nullptr;
        for(auto t: _children) {
            if(t)
                result = t->eval(env);
        }
        return result;
    }
    std::string toString() {
        std::string result;
        for(auto child:_children) {
            result+=child->toString()+" ";
        }
        return result;
    }
};

class IfStmnt : public ASTList
{
public:
	IfStmnt(const std::vector<ASTree*>&c):ASTList(c){}
	
    ASTree* condition(int n) {
        return child(n);
    }
    ASTree* thenBlock(int n) {
        return child(n);
    }
	ASTree* elseBlock() {
		return numChildren() > 2 ? child(numChildren()-1) : nullptr;
	}
	std::string toString() {
        std::string str;
         for(int index=1; index<numChildren()-2;index+=2) {
            str+=thenBlock(index)->toString()+" "+"orif ";
         }
		return ")if " + condition(0)->toString() + " " + str + " else " + elseBlock()->toString() + ")";
	}
    virtual Object* eval(Enviroment& env) {
       /* auto c = (condition())->eval(env);
        if(c&&dynamic_cast<ObjectSub<int>*>(c)->value!=false)
            return (thenBlock())->eval(env);
        else {
            auto b = elseBlock();
            if(b)
                return b->eval(env);
            else
                return nullptr;
        }*/
        for(int index=0; index<numChildren()-1;index+=2) {
            auto c = condition(index)->eval(env);
            if(c&&dynamic_cast<ObjectSub<int>*>(c)->value!=false)
                return (thenBlock(index+1))->eval(env);
        }
        auto b = elseBlock();
        if(b)
            return b->eval(env);
        else
            return nullptr;
    }
};


class WhileStmnt : public ASTList
{
public:
	WhileStmnt(const std::vector<ASTree*>&c) : ASTList(c) {

	}
    
    std::string toString() override {
        return "(while " + condition()->toString() + " " + body()->toString() + ")";
    }
    virtual Object* eval(Enviroment& env) override{
        Object* result =nullptr;
        //auto cond =condition()->eval(env);
        while(dynamic_cast<ObjectSub<int>*>(condition()->eval(env))->value) {
            
            result = (body())->eval(env);
        }
        return result;
    }
    
private:
	ASTree* condition() {
		return child(0);
	}

	ASTree* body() {
		return child(1);
	}
};



class NullStmnt : public ASTList
{
public:
	NullStmnt(const std::vector<ASTree*>&c):ASTList(c){}

};


class ParameterList : public ASTList
{
public:
    ParameterList(const std::vector<ASTree*> c):ASTList(c){}
    std::string name(int i) {
        return ((ASTLeaf*)child(i))->token()->getText();
    }
    int size() {
        return numChildren();
    }
    std::string toString() {
        std::string result;
        for(auto child:_children) {
            result+=child->toString()+" ";
        }
        return result;
    }
    void eval(Enviroment& env,int index, Object*value) {
        env.putNew(name(index), value);
    }
};

class Fun :public ASTList
{
public:
    Fun(const std::vector<ASTree*>& c):ASTList(c) {}
    virtual ParameterList* parameters() {
        return dynamic_cast<ParameterList*>(child(0));
    }
    virtual BlockStmnt* body() {
        return dynamic_cast<BlockStmnt*>(child(1));
    }
    std::string toString() override{
        return "(fun "+ parameters()->toString()+" "+body()->toString();
    }
    virtual Object* eval(Enviroment& env) override;
};

class DefStmnt :public Fun
{
public:
    DefStmnt(const std::vector<ASTree*> c):Fun(c){}
    std::string name() {
        return ((ASTLeaf*)child(0))->token()->getText();
    }
    ParameterList* parameters() override {
        return dynamic_cast<ParameterList*>(child(1));
    }
    BlockStmnt* body() override{
        return dynamic_cast<BlockStmnt*>(child(2));
    }
       std::string toString() override{
        return "(def "+ name() +" "+parameters()->toString()+" "+body()->toString();
    }
    virtual Object* eval(Enviroment& env) override;
};

class ClassBody: public ASTList
{
public:
    ClassBody(std::vector<ASTree*>& c): ASTList(c){}
    virtual Object* eval(Enviroment& env) override;

};

class ClassStmnt: public ASTList
{
public:
    ClassStmnt(const std::vector<ASTree*>& c):ASTList(c){}
    std::string name() {
        std::string re = (dynamic_cast<ASTLeaf*>(child(0)))->token()->getText();
        return re;
    }
    std::string superClass() {
        return numChildren()<3?"NonexsitedSuperClass!":(static_cast<ASTLeaf*>(child(1)))->token()->getText();
    }
    
    ClassBody* body() {
        return dynamic_cast<ClassBody*>(child(numChildren()-1));
    }
    
    std::string toString() override{
        auto parent = superClass();
        if(parent.empty())
            parent ="*";
        return "( class "+ name()+" "+parent+" "+body()->toString() +")\n";
    }
    virtual Object* eval(Enviroment& env) override;
};

class Dot :public Postfix
{
public:
    Dot(const std::vector<ASTree*>& c) : Postfix(c){
        
    }
    std::string name() {
        return static_cast<ASTLeaf*>(child(0))->token()->getText();
    }
    std::string toString() override {
        return "."+name();
    }
    virtual Object* eval(Enviroment& env,Object* value) override;
};

class NewObject: public ASTList
{
public:
    NewObject(const std::vector<ASTree*>& c):ASTList(c){}
    
    virtual Object* eval(Enviroment& env) override;
    
   
};

class ArrayRV : public ASTList
{
public:
    ArrayRV(const std::vector<ASTree*>& c):ASTList(c) {
        
    }
    unsigned int size() {
        return numChildren();
    }
    virtual Object* eval(Enviroment& env) override {
        auto sz = size();
        Object* *ary = new Object*[sz];
        int i = 0;
        for(auto c: _children) {
            ary[i++] = c->eval(env);
        }
        return new ArrayWrapper(ary,i);
    }
};

class ArraySuffix : public Postfix
{
public:
    ArraySuffix(const std::vector<ASTree*>& c):Postfix(c) {
        
    }
    ASTree* index() {
        return child(0);
    }
    virtual std::string toString() override {
        return "[" + index()->toString()+"]";
    }
    virtual Object* eval(Enviroment& env, Object* value) override {
        auto cValue = dynamic_cast<ArrayWrapper*>(value);
        if(!cValue) {
            PRINT_ERR_ARGS("Bad array type!");
            return nullptr;
        }
        auto idex = index()->eval(env);
        auto cIdex = dynamic_cast<ObjectSub<int>*>(idex);
        if(!cIdex) {
            PRINT_ERR_ARGS("bad array index!");
            return nullptr;
        }
        return (cValue->array)[cIdex->value];
    }
};

class ForStmnt : public ASTList
{
public:
    ForStmnt(const std::vector<ASTree*>& c): ASTList(c) {
        
    }
    virtual std::string toString() override {
        return "For statement: "+ initSt()->toString()+" "+condition()->toString()+" "+
        iteraEpr()->toString()+" "+body()->toString();
    }
private:
    ASTree* initSt() {
        return child(0);
    }
    ASTree* condition() {
        return child(1);
    }
    ASTree* iteraEpr() {
        return child(2);
    }
    ASTree* body() {
        return child(3);
    }
    virtual Object* eval(Enviroment& env) override {
        initSt()->eval(env);
        Object* result = nullptr;
        while(dynamic_cast<ObjectSub<int>*>(condition()->eval(env))->value) {
            result = body()->eval(env);
            iteraEpr()->eval(env);
        }
        return result;
    }
    
};

class Foreach : public ASTList
{
public:
    Foreach(const std::vector<ASTree*>&c): ASTList(c) {
        
    }
private:
    ASTree* variable () {
        return child(0);
    }
    ASTree* array() {
        return child(1);
    }
    ASTree* body() {
        return child(2);
    }
    virtual Object* eval(Enviroment& env) override {
        auto var = variable();
        auto castVar = dynamic_cast<Name*>(var);
        if(!castVar) {
            PRINT_ERR_ARGS("Assigned variable in foreach is Not Name type!");
            return nullptr;
        }
        auto ar = array()->eval(env);
        auto castArray = dynamic_cast<ArrayWrapper*>(ar);
        if(!castArray) {
            PRINT_ERR_ARGS("Bad type for ArrayWrapper!");
            return nullptr;
        }
        auto aPointer = castArray->array;
        auto bk = body();
        Object* result = nullptr;
        for(int i=0; i< castArray->memberNumber();i++) {
            env.put(castVar->name(), aPointer[i]);
            result=bk->eval(env);
        }
        return result;
    }

};
#endif // !AST_STRUCTURES_H_


