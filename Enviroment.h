//
//  Enviroment.h
//  Stone
//
//  Created by Heran Gao on 6/15/17.
//  Copyright © 2017 Heran.Gao. All rights reserved.
//

#ifndef Enviroment_h
#define Enviroment_h
#include<map>
#include<iostream>
#include"Informations.h"
#include"Object.h"


class Enviroment
{

public:
   // virtual void put(const std::string name,int v) = 0;
   // virtual void put(const std::string name,std::string v) = 0;
    virtual void put(const std::string name,Object*v) = 0;
    virtual Object* get(const std::string &name)=0;
    virtual Enviroment* where(const std::string& name) = 0;
    virtual void putNew(const std::string& name, Object* _value)=0;
    virtual void setOuter(Enviroment* e) = 0;
protected:
    std::map<std::string,Object*> values;
    
    static bool isEmpty(const Object* v) {
        return v==nullptr;
    }

};

class BasicEnv : public Enviroment
{
public:
    BasicEnv() = default;
    void put(const std::string name,Object* v) {
        values[name]= v;
    }

       Object* get(const std::string &name) {
        return values.at(name);
    }
    Enviroment* where(const std::string& name){
        PRINT_ERR_ARGS("BasicEnv should Not use where function!");
        return nullptr;
    }
    
    void setOuter(Enviroment* e) {
        PRINT_ERR_ARGS("BasicEnv should Not use setOuter function!");
    }
    template<typename T>
    static T assignTo(T& obj,std::pair<std::string,int>&& p) {
        obj = std::get<T>(p);
        return obj;
    }
protected:
    void putNew(const std::string& name, Object* _value) {
        PRINT_ERR_ARGS("BasicEnv should Not use putNew function!");
    }

};

class NestedEnv : public Enviroment
{
public:
    NestedEnv()= default;
    NestedEnv(Enviroment* e) {
        outer = e;
    }
    void setOuter(Enviroment* e) override{
        outer = e;
    }
    Object* get(const std::string &name) override{
        auto v = values.find(name);
        if(v==values.end()&&outer)
            return outer->get(name);
        else if(v==values.end()&&!outer)
            return nullptr;
        else
            return v->second;
    }
    void put(const std::string name,Object* p) override{
        auto cEnv = where(name);
        if(cEnv) {
            if(!dynamic_cast<NestedEnv*>(cEnv)) {
               cEnv->put(name, p);
            }
            else {
               cEnv->putNew(name,p);
                
            }
            
        }
        else {
            putNew(name, p);
        }
    }
    Enviroment* where(const std::string& name) override{
        if(values.find(name)!=values.end()) {
            return this;
        }
        else if(outer == nullptr)
            return nullptr;
        else
            return outer->where(name);
            
    }
protected:
    Enviroment* outer = nullptr;
    void putNew(const std::string& name, Object* _value) override{
        values[name] = _value;
    }
};


#endif /* Enviroment_h */
