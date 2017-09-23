#include"ASTstructures.h"
#include"Function.h"
#include "Class.h"
std::vector<ASTree*> ASTLeaf::empty{};

std::string ASTList::toString() {
	std::string result{ "(" };
	std::string sep{ "" };
	for (auto t : _children) {
		result += sep;
		sep = " ";
		result += t->toString();
	}
	return result += ")";
}

std::string ASTList::location() {
    std::string s;
	for (auto t : _children) {
		 s = t->location();
		return s;
	}
    return "error";
}

Object* BinaryExpr::computeAssign(Enviroment& env,Object* rvalue) {
    auto l =left();
    auto acPE = dynamic_cast<PrimaryExpr*>(l);
    if(acPE) {
        auto acd = dynamic_cast<Dot*>(acPE->postfix(0));
        if(acPE->hasPostfix(0)&&acd) {
            auto t = acPE->evalSubExpr(env, 1);
            auto act = dynamic_cast<ObjectSub<Instance*>*>(t);
            if(act) {
                auto name = acd->name();
                 act->value->write(name, rvalue);
                return rvalue;
                
            }
        }
        auto ap = dynamic_cast<ArraySuffix*>(acPE->postfix(0));
        if(acPE->hasPostfix(0)&&ap) {
            auto t = acPE->evalSubExpr(env, 1);
            auto act = dynamic_cast<ArrayWrapper*>(t);
            if(act) {
                auto indexObj = ap->index()->eval(env);
                auto cIndex = dynamic_cast<ObjectSub<int>*>(indexObj);
                if(!cIndex) {
                    PRINT_ERR_ARGS("should be ObjectSub<int>*!");
                    return nullptr;
                }
                (act->array)[cIndex->value] = rvalue;
                return rvalue;
            }
        }
        
    }
    auto afterCastN = dynamic_cast<Name*>(l);
    
    if(afterCastN) {
        env.put(afterCastN->name(), rvalue);
        return rvalue;
    }
    
    else {
        PRINT_ERR_ARGS("left operator should be either NameType or Dot!");
        return nullptr;
    }
}


Object* BinaryExpr::computeINumber(int l, std::string op,int r) {
    if(op=="+")
        return new ObjectSub<int>(l+r);
    else if(op=="-")
        return new ObjectSub<int>(l-r);
    else if(op=="*")
        return new ObjectSub<int>(l*r);
    else if(op=="/")
        return new ObjectSub<int>(l/r);
    else if(op=="%")
        return new ObjectSub<int>(l%r);
    else if(op=="==")
        return new ObjectSub<int>(l == r? true:false);
    else if(op=="!=")
        return new ObjectSub<int>(l == r? false:true);
    else if(op==">")
        return new ObjectSub<int>(l > r? true:false);
    else if(op=="<")
        return new ObjectSub<int>(l < r? true:false);
    else if(op=="<=")
        return new ObjectSub<int>(l <= r? true:false);
    else if(op==">=")
        return new ObjectSub<int>(l >=r? true:false);
    else if(op=="&&")
        return new ObjectSub<int>((l==1)&&(r==1)? true:false);
    else if(op=="||")
        return new ObjectSub<int>((l==1)||(r==1)? true:false);
    
    else {
        PRINT_ERR_ARGS("Invalid operator!");
        return nullptr;
    }
}

Object* BinaryExpr::computeDNumber(double l, std::string op,double r) {
    if(op=="+")
        return new ObjectSub<double>(l+r);
    else if(op=="-")
        return new ObjectSub<double>(l-r);
    else if(op=="*")
        return new ObjectSub<double>(l*r);
    else if(op=="/")
        return new ObjectSub<double>(l/r);
    else if(op=="==")
        return new ObjectSub<double>(l == r? true:false);
    else if(op=="!=")
        return new ObjectSub<double>(l == r? false:true);
    else if(op==">")
        return new ObjectSub<double>(l > r? true:false);
    else if(op=="<")
        return new ObjectSub<double>(l < r? true:false);
    else if(op=="<=")
        return new ObjectSub<double>(l <= r? true:false);
    else if(op==">=")
        return new ObjectSub<double>(l >=r? true:false);
    
    else {
        PRINT_ERR_ARGS("Invalid operator!");
        return nullptr;
    }
}

Object* BinaryExpr::computeOp(Object* l, std::string op, Object* r){
    auto al = dynamic_cast<ObjectSub<int>*>(l);
    auto ar = dynamic_cast<ObjectSub<int>*>(r);
    auto dl = dynamic_cast<ObjectSub<double>*>(l);
    auto dr = dynamic_cast<ObjectSub<double>*>(r);
    auto sl = dynamic_cast<ObjectSub<std::string>*>(l);
    auto sr = dynamic_cast<ObjectSub<std::string>*>(r);
    if(al) {
        if(ar) {
            return computeINumber(al->value, op, ar->value);

        }
        else if(dr) {
            return computeDNumber(double(al->value), op, (dr->value));
        }
        else if(sr) {
            return new ObjectSub<std::string>(al->toString()+sr->value);
        }
        else {
            PRINT_ERR_ARGS("Invalid type!");
            return nullptr;
        }
    }
    else if(dl) {
        if(ar) {
            return computeDNumber(dl->value, op, double(ar->value));
            
        }
        else if(dr) {
            return computeDNumber(dl->value, op, (dr->value));
        }
        else if(sr) {
            return new ObjectSub<std::string>(dl->toString()+sr->value);
        }
        else {
            PRINT_ERR_ARGS("Invalid type!");
            return nullptr;
        }
    }
    else if(sl) {
        if(ar) {
             return new ObjectSub<std::string>(sl->value+ar->toString());
            
        }
        else if(dr) {
            return new ObjectSub<std::string>(sl->value+dr->toString());
        }
        else if(sr) {
            return new ObjectSub<std::string>(sl->value+sr->value);
        }
        else {
            PRINT_ERR_ARGS("Invalid type!");
            return nullptr;
        }
    }

    else {
        if(op=="+") {
            return new ObjectSub<std::string>(dynamic_cast<ObjectSub<std::string>*>(l)->value+dynamic_cast<ObjectSub<std::string>*>(r)->value);
        }
        else if(op=="==") {
            
            return new ObjectSub<int>(l->toString()==r->toString());
        }
        
        else {
            PRINT_ERR_ARGS("Invalid type!");
            return nullptr;
        }
    }
}

Object* BinaryExpr::eval(Enviroment&env) {
    auto op = oPerator();
    if(op == "=") {
        auto rightP = (right())->eval(env);
        return computeAssign(env, rightP);
    }
    else {
        auto l = (left())->eval(env);
        auto r = (right())->eval(env);
        return computeOp(l, op, r);
    }
}

Object* Fun::eval(Enviroment& env){
    return new ObjectSub<Function*>(new Function(parameters(),body(),env));
}

Object* DefStmnt::eval(Enviroment& env){
    env.putNew(name(), new ObjectSub<Function*>(new Function(parameters(),body(),env)));
    return new ObjectSub<std::string>(name());
}

Object* Arguments::eval(Enviroment& callerEnv,Object* value ) {
    auto func = dynamic_cast<ObjectSub<Function*>*>(value);
    if(!func) {
        PRINT_ERR_ARGS("Bad Function!");
        return nullptr;
    }
    auto params = func->value->parameters();
    if(size()!=params->size()) {
        PRINT_ERR_ARGS("Bad number of arguments!");
        return nullptr;
    }
    auto newEnv = func->value->makeEnv();
    int num = 0;
    for(auto s=this->children();s!=_children.end();s++) {
        dynamic_cast<ParameterList*>(params)->eval(*newEnv,num++,(*s)->eval(callerEnv));
    }
    return func->value->body()->eval(*newEnv);
}

Object* FixedArguments::eval(Enviroment &callerEnv, Object *value) {
    auto func = dynamic_cast<NativeFunction*>(value);
    if(!func)
        return Arguments::eval(callerEnv, value);
    if(func->toString()== "print") {
         std::cout<<(this->child(0)->eval(callerEnv))->toString();
        return new ObjectSub<Comment>(Comment("std::cout"));
    }
    else if(func->toString() == "time") {
        auto duration = time(nullptr) - ((ObjectSub<long>*)callerEnv.get("startime"))->value;
        return new ObjectSub<long>(duration);
    }
    else if(func->toString() == "scanf") {
        std::string input;
        std::cin>>input;
        auto castName = dynamic_cast<Name*>(child(0));
        if(!castName) {
            PRINT_ERR_ARGS("Bad Name type!");
            return nullptr;
        }
        if(input.at(0)=='"') {
            input = input.substr(1,input.length()-2);
            auto re =  new ObjectSub<std::string>(input);
            callerEnv.put(castName->name(),re);
            return re;
        }
        else {
            auto re = new ObjectSub<int>(std::stoi(input));
            callerEnv.put(castName->name(),re);
            return re;
        }
    }
    else {
        PRINT_ERR_ARGS("Neither User-defined function nor local function!");
        return nullptr;
    }
    
}


Object* ClassStmnt::eval(Enviroment &env) {
    auto v = new ObjectSub<ClassInfo*>(new ClassInfo(this,&env));
    auto c =name();
    env.put(name(),v);
   // return new ObjectSub<std::string>(name());
    return v;
}

Object* ClassBody::eval(Enviroment &env) {
    for(auto t:_children) {
        t->eval(env);
    }
    return nullptr;
}

Object* Dot::eval(Enviroment &env,Object* value) {
    auto member = name();
    auto ins = dynamic_cast<ObjectSub<Instance*>*>(value);
    if(!ins) {
        PRINT_ERR_ARGS("Bad member access!");
        return nullptr;
    }
    else {
        return ins->value->read(member);
    }
}

Object* NewObject::eval(Enviroment &env) {
    auto re = child(0)->eval(env);
    auto classInformObj =dynamic_cast<ObjectSub<ClassInfo*>*>(re);
    if(!classInformObj) {
        PRINT_ERR_ARGS("can't find the given ClassInfo!");
        return nullptr;
    }
    else {
        Enviroment* nenv = new NestedEnv(classInformObj->value->enviroment());
        ClassInfo* sc = classInformObj->value;
        ClassInfo::initObj(sc, nenv);
        auto newObj = new ObjectSub<Instance*>(new Instance(nenv));
        newObj->value->putMember("this", newObj);
        return newObj;
    }
}

