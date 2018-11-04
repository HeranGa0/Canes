//
//  Function.h
//  Stone
//
//  Created by Heran Gao on 6/26/17.
//  Copyright © 2017 Heran.Gao. All rights reserved.
//

#ifndef Function_h
#define Function_h
#include "Enviroment.h"
#include"ASTstructures.h"
#include <tuple>
#include"QLineEdit"
#include"QEventLoop"
#include"Class.h"
class Function
{
public:
    Function(ParameterList* parameters, BlockStmnt* _body, Enviroment& _env) {
        param = parameters;
        mbody = _body;
        menv = &_env;
    }
    auto parameters() {
        return param;
    }
    auto body() {
        return mbody;
    }
    auto makeEnv() {
        return new NestedEnv(menv);
    }
    std::string toString() {
        return "<fun:" + param->toString() + mbody->toString()+">";
    }
protected:
    ParameterList* param = nullptr;
    BlockStmnt* mbody = nullptr;
    Enviroment* menv=nullptr;
    
};

/*
template<>
class ObjectSub<Function>: public Object
{
public:
    Function* fptr = nullptr;
    ObjectSub<Function>(Function* ptr):fptr(ptr) {}
    ObjectSub<Function>& operator = (Function* ptr){
        fptr = ptr;
        return *this;
    }
    virtual std::string toString() override {
        return fptr->toString();
    }
};*/

class NativeFunction : public Object
{
public:
    NativeFunction(std::string name):_name(name) {
        
    }
    virtual std::string toString() override{
        return _name;
    }
private:
    std::string _name;
};

class PrintFunction:public NativeFunction
{
public:
    PrintFunction(std::string name,QLineEdit* output):NativeFunction(name),_output(output) {

    }
   QLineEdit* _output;
};
class ScanfFunction:public NativeFunction
{
public:
    ScanfFunction(std::string name,QLineEdit* output,QEventLoop*timer):NativeFunction(name),_output(output),_timer(timer) {

    }
   QLineEdit* _output;
    QEventLoop* _timer;
   std::string afterInput="";
};

class NewInstanceFunction: public NativeFunction
{
public:
    NewInstanceFunction(std::string name,Object* ci):NativeFunction(name),_ci(ci) {

    }
    Object* _ci;
};


#endif /* Function_h */
