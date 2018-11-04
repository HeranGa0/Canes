#ifndef AST_STRUCTURES_H_
#define AST_STRUCTURES_H_
#include<vector>
#include<algorithm>
#include<iostream>
#include"Token.h"
#include"Enviroment.h"
#include"Informations.h"
#include<QDebug>
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
    virtual std::string name() {
        return "ASTree";
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
        return new NullObject();
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
        return new NullObject();
    }
    std::vector<ASTree*> getChildren() {
        return _children;
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
    float value() {
        return token()->getDnumber();
    }
    virtual Object* eval(Enviroment&env) {
        return new ObjectSub<float>(value());
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
class NilLiteral : public ASTLeaf
{
public:
    NilLiteral(Token* t) :ASTLeaf(t) {}
    std::string value() {
        return token()->getText();
    }
    virtual Object* eval(Enviroment&env) {
        return new NullObject();
    }
};

class Trueiteral : public ASTLeaf
{
public:
    Trueiteral(Token* t) :ASTLeaf(t) {}
    std::string value() {
        return token()->getText();
    }
    virtual Object* eval(Enviroment&env) {
        return new TrueObject();
    }
};

class Falseiteral : public ASTLeaf
{
public:
    Falseiteral(Token* t) :ASTLeaf(t) {}
    std::string value() {
        return token()->getText();
    }
    virtual Object* eval(Enviroment&env) {
        return new FalseObject();
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
    Object* computeDNumber(float l, std::string op,float r);
    

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

class Operators:public ASTList
{
public:

    Operators(const std::vector<ASTree*> &c):ASTList(c){}
    virtual Object* eval(Enviroment& env);
private:

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
    auto cBegin() {
        return _children.begin();
    }
    auto cEnd() {
        return _children.end();
    }
};


class PrimaryExpr : public ASTList
{
private:
    PrimaryExpr(const std::vector<ASTree*>& c):ASTList(c) {
        for(int index=1;index<numChildren();index++) {
           if(child(index)->toString()==".new") {
               newIndex = index;
           }
        }
    }
    int newIndex;

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
        return child(1);
	}

	std::string toString() {
		return "-" + operand()->toString();
	}
    virtual Object* eval(Enviroment& env) {
        auto v = (operand())->eval(env);
        if(auto iv=dynamic_cast<ObjectSub<int>*>(v)) {
            iv->value = (-1)*(iv->value);
            return iv;
        }
        else {
            PRINT_ERR_ARGS("NegativeExpr should be IntType!")
            return new NullObject();
        }
            
    }
};


class NotExpr : public ASTList
{
public:
    NotExpr(const std::vector<ASTree*>&c) :ASTList(c) {

    }

    ASTree* operand() {
        return child(1);
    }

    std::string toString() {
        return "!" + operand()->toString();
    }
    virtual Object* eval(Enviroment& env) {
        auto v = (operand())->eval(env);
        qDebug()<<QString::fromStdString(v->toString());
        if(auto iv=dynamic_cast<TrueObject*>(v)) {
            return new FalseObject();
        }
        else if(auto iv=dynamic_cast<FalseObject*>(v)) {
            return new TrueObject();
        }
        else {
            PRINT_ERR_ARGS("should be TrueObject or FalseObject!")
            return new NullObject();
        }

    }
};

class ReturnStmnt: public ASTList
{
public:
    ReturnStmnt(const std::vector<ASTree*>&c) :ASTList(c) {

    }
    std::string toString() {
        return "return " +child(0)->toString();
    }
    virtual Object* eval(Enviroment&env) {
        return new ReturnObject(child(0)->eval(env));
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
            auto reobj=dynamic_cast<ReturnObject*>(result);
            if(reobj)
                return reobj;
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
        return "if " + condition(0)->toString() + " " + thenBlock(1)->toString();
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
            if(c&&dynamic_cast<TrueObject*>(c))
                return (thenBlock(index+1))->eval(env);
        }
        auto b = elseBlock();
        if(b)
            return b->eval(env);
        else
            return new NullObject();
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
        while(dynamic_cast<TrueObject*>(condition()->eval(env))) {
            
            result = (body())->eval(env);
            if(dynamic_cast<BreakObject*>(result)) {
                break;
            }
            auto reobj=dynamic_cast<ReturnObject*>(result);
            if(reobj)
                return reobj->rValue;
        }
        if(result)
            return result;
        else
            return new NullObject();
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
    std::string name() override {
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
    NewObject(const std::vector<ASTree*>& c,Object* ciobj,FixedArguments* p):ASTList(c),_ciobj(ciobj),_p(p){}
    
    virtual Object* eval(Enviroment& env) override;
private:
    Object* _ciobj = nullptr;
    FixedArguments* _p;
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
        auto arrayWrapper = new ArrayWrapper();
        /*if(size()==0) {
            return arrayWrapper;
        }
      */
        //Object* *ary = new Object*[sz];
        int i = 0;
        for(auto c: _children) {
            arrayWrapper->exArray [i++] = c->eval(env);
        }
        return  arrayWrapper;
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
            return new NullObject();
        }
        auto idex = index()->eval(env);
        auto cIdex = dynamic_cast<ObjectSub<int>*>(idex);
        /*if(!cIdex) {
            PRINT_ERR_ARGS("bad array index!");
            return nullptr;
        }*/
        if(cIdex){
                return (cValue->exArray)[(cIdex->value)];
            }

        auto sIdx = dynamic_cast<ObjectSub<std::string>*>(idex);
        if(sIdx) {
            return (cValue->acArray)[(sIdx->value)];
        }
    }
};


class BreakStmnt:public ASTLeaf
{
public:
    BreakStmnt(Token * t):ASTLeaf(t) {

    }

    virtual Object* eval(Enviroment&env) {
        return new BreakObject();
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
        while(dynamic_cast<TrueObject*>(condition()->eval(env))) {
            result = body()->eval(env);
            if(dynamic_cast<BreakObject*>(result)) {
                break;
            }
            /*if(dynamic_cast<ReturnObject*>(result))
                return result;*/

            iteraEpr()->eval(env);
        }
        if(result)
            return result;
        else
            return new NullObject();
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
        auto aPointer = castArray->exArray;
        auto bk = body();
        Object* result = nullptr;
        for(int i=0; i< castArray->memberNumber();i++) {
            env.put(castVar->name(), castArray->atInt(i));
            result=bk->eval(env);
            if(dynamic_cast<BreakObject*>(result)) {
                break;
            }
        }
        if(result)
            return result;
        else
            return new NullObject();
    }

};
#endif // !AST_STRUCTURES_H_


