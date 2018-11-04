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
#include<QDebug>
class ClassInfo
{
protected:
    ClassStmnt* definition;
    Enviroment* _enviroment;
    ClassInfo* _superClass;
public:
    ClassInfo(ClassStmnt* cs, Enviroment* env):definition(cs),_enviroment(env){
        Object* obj = env->get(cs->superClass());
        //qDebug()<<QString::fromStdString(obj->toString());
        if(!obj)
            _superClass=nullptr;
        else if((_superClass=((dynamic_cast<ObjectSub<ClassInfo*>*>(obj))->value))) {
            
        }
        else {
            PRINT_ERR_ARGS("Unknown superclass "+cs->toString());
        }
    }
    bool ifExistingSuperclass(std::string cn) {
        if(name()!=cn) {
            if(superClass()!=nullptr) {
                return superClass()->ifExistingSuperclass(cn);
            }
            else
                return false;
        }
        else
            return true;

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
    DefStmnt* findFunction(std::string fName) {
        for(ASTree* m:body()->getChildren()) {

            auto isFunc= dynamic_cast<DefStmnt*>(m);
            if(isFunc) {
                if(isFunc->name()==fName)
                return isFunc;
            }

        }
        return nullptr;
    }

    static void initObj(ClassInfo* ci, Enviroment* cEnv,FixedArguments* fa);

};

class Instance
{
public:
    Instance(Enviroment* e,ClassInfo* ci):env(e),_ci(ci){}
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
    ClassInfo* _ci;
protected:
    Enviroment* env;
    Enviroment* getEnv(std::string member) {
        Enviroment* e = env->where(member);
        if(e!=nullptr && e == env)
            return e;
        else {
            PRINT_ERR_ARGS("This enviroment could Not find the given member: "+member);
            return nullptr;
        }
    }
   };

#endif /* Class_h */
