//
//  Class.h
//  Stone
//
//  Created by Heran Gao on 7/22/17.
//  Copyright © 2017 Heran.Gao. All rights reserved.
//

#ifndef Class_h
#define Class_h
#include "Enviroment.h"
#include"ASTstructures.h"
class ClassInfo
{
protected:
    ClassStmnt* definition;
    Enviroment* _enviroment;
    ClassInfo* _superClass;
public:
    ClassInfo(ClassStmnt* cs, Enviroment* env):definition(cs),_enviroment(env){
        Object* obj = env->get(cs->superClass());
        if(!obj)
            _superClass=nullptr;
        else if((_superClass=((dynamic_cast<ObjectSub<ClassInfo*>*>(obj))->value))) {
            
        }
        else {
            PRINT_ERR_ARGS("Unknown superclass "+cs->toString());
        }
    }
    std::string name() {
        return definition->name();
    }
    ClassInfo* superClass() {
        return _superClass;
    }
    ClassBody* body() {
        return definition->body();
    }
    Enviroment* enviroment() {
        return _enviroment;
    }
    std::string toString() {
        return "< class "+name()+" >";
    }
    static void initObj(ClassInfo* ci, Enviroment* cEnv) {
        if(ci->superClass()) {
            initObj(ci->superClass(),cEnv);
        }
        ci->body()->eval(*cEnv);
    }
};

class Instance
{
public:
    Instance(Enviroment* e):env(e){}
    std::string toString() {
        return "<Instance:" + std::to_string(typeid(this).hash_code());
    }
    
    Object* read(std::string member) {
        return getEnv(member)->get(member);
    }
    void write(std::string member, Object* value) {
        getEnv(member)->putNew(member, value);
    }
    void putMember(std::string name,Object* value) {
        env->putNew(name, value);
    }
protected:
    Enviroment* env;
    Enviroment* getEnv(std::string member) {
        Enviroment* e = env->where(member);
        if(e!=nullptr && e == env)
            return e;
        else {
            PRINT_ERR_ARGS("This enviroment could Not find the given member!");
            return nullptr;
        }
    }
   };

#endif /* Class_h */
