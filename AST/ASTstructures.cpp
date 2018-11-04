#include"ASTstructures.h"
#include"Function.h"
#include "Class.h"
#include<QDebug>
#include <fstream>
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

struct Op_Ci
{
    Op_Ci(ClassInfo*ci1,std::string op,ClassInfo* ci2):_ci1(ci1),_op(op),_ci2(ci2){}
    ClassInfo* _ci1;
    std::string _op;
    ClassInfo* _ci2;

    bool operator< ( const Op_Ci &right)const {
        return !(_ci1->ifExistingSuperclass(right._ci1->name())&&_ci2->ifExistingSuperclass(right._ci2->name())&&_op == right._op);
    }
};

/*static bool operator< ( const Op_Ci&left,const Op_Ci &right) {
    return !(left._ci1->ifExistingSuperclass(right._ci1->name())&&left._ci2->ifExistingSuperclass(right._ci2->name())&&left._op == right._op);
}*/
static std::map<Op_Ci,ObjectSub<Function*>*> oprt_func;

#define LAZY(op)\
std::string operator op ( std::string l, int r)  {\
    return  l + std::to_string(r);\
}\
std::string operator op (  int r,std::string l)  {\
    return  l+ std::to_string(r);\
}\
std::string operator op ( std::string l, float r)  {\
    return  l+ std::to_string(r);\
}\
std::string operator op (float l, std::string  r)  {\
    return  r+ std::to_string(l);\
}

LAZY(+)
LAZY(-)
LAZY(*)
LAZY(/)
#undef LAZY
Object* BinaryExpr::computeAssign(Enviroment& env,Object* rvalue) {
   auto op = oPerator();
    auto l =left();

    auto calcValue = [this,op](Object* l, Object* r)->Object* {

        auto il = dynamic_cast<ObjectSub<int>*>(l);
        auto fl = dynamic_cast<ObjectSub<float>*>(l);
        auto sl = dynamic_cast<ObjectSub<std::string>*>(l);
        auto ir = dynamic_cast<ObjectSub<int>*>(r);
        auto fr = dynamic_cast<ObjectSub<float>*>(r);
        auto sr = dynamic_cast<ObjectSub<std::string>*>(r);
#define SHORTCUTT(L,R)\
    do {\
        if(L) {\
            if(R) {\
                if(op=="+=")\
                return new ObjectSub<decltype(L->value + R->value)>(L->value+R->value);\
               else if(op=="-=")\
                return new ObjectSub<decltype(L->value - R->value)>(L->value - R->value);\
               else if(op=="*=")\
                return new ObjectSub<decltype(L->value * R->value)>(L->value * R->value);\
              else if(op=="/=")\
              return new ObjectSub<decltype(L->value / R->value)>(L->value / R->value);\
               else {\
                PRINT_ERR_ARGS("niether +=, -=, *= nor /=!");\
                return nullptr;\
             }\
            }\
        }\
    } while(0);
SHORTCUTT(il,ir);
SHORTCUTT(il,fr);
SHORTCUTT(il,sr);
SHORTCUTT(fl,ir);
SHORTCUTT(fl,fr);
SHORTCUTT(fl,sr);
SHORTCUTT(sl,ir);
SHORTCUTT(sl,fr);

#undef SHORTCUTT
if(sl&&sr) {
    return  new ObjectSub<std::string>(sl->value+sr->value);
}
return new NullObject();
};



    auto acPE = dynamic_cast<PrimaryExpr*>(l);

    if(acPE) {
        auto acd = dynamic_cast<Dot*>(acPE->postfix(0));
        if(acPE->hasPostfix(0)&&acd) {
            auto t = acPE->evalSubExpr(env, 1);
            auto act = dynamic_cast<ObjectSub<Instance*>*>(t);
            Object* fvalue = nullptr;
            if(act) {
                auto name = acd->name();
                if(op == "=") {
                 act->value->write(name, rvalue);
                fvalue=rvalue;
                }
                else  {
                  fvalue = calcValue(act->value->read(name),rvalue);
                 act->value->write(name, fvalue);
                }
                return fvalue;
                
            }
        }
        auto ap = dynamic_cast<ArraySuffix*>(acPE->postfix(0));

        if(acPE->hasPostfix(0)&&ap) {
            auto t = acPE->evalSubExpr(env, 1);
            auto act = dynamic_cast<ArrayWrapper*>(t);
            if(act) {
                auto indexObj = ap->index()->eval(env);
                auto cIndex = dynamic_cast<ObjectSub<int>*>(indexObj);
                Object* fvalue;
               /* if(!cIndex) {
                    PRINT_ERR_ARGS("should be ObjectSub<int>*!");
                    return nullptr;
                }*/
                if(cIndex) {
                   /* if(cIndex->value<act->memberNumber()) {
                        if(op=="=") {
                        (act->array)[cIndex->value] = rvalue;
                         fvalue = rvalue;
                        }
                        else {
                            fvalue = calcValue((act->array)[cIndex->value],rvalue);
                           (act->array)[cIndex->value] = fvalue;
                        }

                    }
                    else if(cIndex<){*/
                        if(op=="=") {
                        (act->exArray)[cIndex->value] = rvalue;
                            fvalue=rvalue;
                        }
                        else {
                            fvalue = calcValue((act->exArray)[cIndex->value],rvalue);
                           (act->exArray)[cIndex->value] = fvalue;


                    }


                    return fvalue;
                }

                auto sIndex =  dynamic_cast<ObjectSub<std::string>*>(indexObj);
                if(sIndex) {
                    if(op=="=") {
                    (act->acArray)[sIndex->value] = rvalue;
                     fvalue = rvalue;
                     qDebug()<<QString::fromStdString(rvalue->toString());
                    }
                    else {
                        fvalue = calcValue((act->acArray)[sIndex->value],rvalue);
                        (act->acArray)[sIndex->value] = fvalue;
                         qDebug()<<QString::fromStdString(fvalue->toString());
                    }

                    return fvalue;
                }
                else {
                    PRINT_ERR_ARGS("neither is sIndex nor sIndex!");
                return new NullObject();
                }
            }
        }
        
    }
    auto afterCastN = dynamic_cast<Name*>(l);
    
    if(afterCastN) {
        if(op!="=") {
            auto fv = calcValue(afterCastN->eval(env),rvalue);
            env.put(afterCastN->name(),fv);
            return fv;
        }
        env.put(afterCastN->name(), rvalue);
        return rvalue;
    }
    
    else {
        PRINT_ERR_ARGS("left operator should be either NameType or Dot!");
        return new NullObject();
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
    else if(op=="<") {
        Object* ptr = nullptr;
        return l < r ? (Object*) (new TrueObject()):(Object*) (new FalseObject());
    }
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
        return new NullObject();
    }
}

Object* BinaryExpr::computeDNumber(float l, std::string op,float r) {
    if(op=="+")
        return new ObjectSub<float>(l+r);
    else if(op=="-")
        return new ObjectSub<float>(l-r);
    else if(op=="*")
        return new ObjectSub<float>(l*r);
    else if(op=="/")
        return new ObjectSub<float>(l/r);
    else if(op=="==")
        return new ObjectSub<float>(l == r? true:false);
    else if(op=="!=")
        return new ObjectSub<float>(l == r? false:true);
    else if(op==">")
        return new ObjectSub<float>(l > r? true:false);
    else if(op=="<")
        return new ObjectSub<float>(l < r? true:false);
    else if(op=="<=")
        return new ObjectSub<float>(l <= r? true:false);
    else if(op==">=")
        return new ObjectSub<float>(l >=r? true:false);
    
    else {
        PRINT_ERR_ARGS("Invalid operator!");
        return new NullObject();
    }
}

Object* BinaryExpr::computeOp(Object* l, std::string op, Object* r){

    auto cl = dynamic_cast<ObjectSub<Instance*>*>(l);
    auto cr = dynamic_cast<ObjectSub<Instance*>*>(r);
    if(cl&&cr) {
        ObjectSub<Function*>* funct = oprt_func.at(Op_Ci(cl->value->_ci,op,cr->value->_ci));
        if(!funct) {
            PRINT_ERR_ARGS("Bad oprt_func!");
            return new NullObject();
        }

        auto params = funct->value->parameters();

        auto newEnv = funct->value->makeEnv();
        int num = 0;

            dynamic_cast<ParameterList*>(params)->eval(*newEnv,0,l);
            dynamic_cast<ParameterList*>(params)->eval(*newEnv,1,r);
        auto re =funct->value->body()->eval(*newEnv);
        auto result = dynamic_cast<ReturnObject*>(re);
        if(result)
            return result;
        else
            return re;

    }


        auto tl = dynamic_cast<TrueObject*>(l);
        auto tr = dynamic_cast<TrueObject*>(r);
        auto fal = dynamic_cast<FalseObject*>(l);
        auto far = dynamic_cast<FalseObject*>(r);
        auto nl = dynamic_cast<NullObject*>(l);
        auto nr = dynamic_cast<NullObject*>(r);
        if(op=="&&") {
            if(!((tl||fal)&&(tr||far))) {
                PRINT_ERR_ARGS("arguments must be both bool types!");
                return new NullObject;
            }
            if(tl&&tr) {
                return new TrueObject;
            }
            else {
                return new FalseObject;
            }
        }
         if(op=="||") {
            if(!((tl||fal)&&(tr||far))) {
                PRINT_ERR_ARGS("arguments must be both bool types!");
                return new NullObject;
            }
            else if(tl||tr) {
                return new TrueObject;
            }
            else
                return new FalseObject;
        }
         if(tl||tr||fal||far) {
             if(op!="==") {
                 PRINT_ERR_ARGS("Invalid operator between bool type!");
                 return new NullObject;
             }
             if((tl&&tr)||(fal&&far)) {
                 return new TrueObject;
             }
             else {
                 return new FalseObject;
             }
         }
        if(nl||nr) {
            if(op!="!=") {
                PRINT_ERR_ARGS("NullObject can only operate with !=");
                return new NullObject;
            }
            else {
                return new FalseObject;
            }
        }


    auto al = dynamic_cast<ObjectSub<int>*>(l);
    auto ar = dynamic_cast<ObjectSub<int>*>(r);
    auto dl = dynamic_cast<ObjectSub<float>*>(l);
    auto dr = dynamic_cast<ObjectSub<float>*>(r);
    auto sl = dynamic_cast<ObjectSub<std::string>*>(l);
    auto sr = dynamic_cast<ObjectSub<std::string>*>(r);
    if(al) {
        if(ar) {
            return computeINumber(al->value, op, ar->value);

        }
        else if(dr) {
            return computeDNumber(float(al->value), op, (dr->value));
        }
        else if(sr) {
            return new ObjectSub<std::string>(al->toString()+sr->value);
        }
        else {
            PRINT_ERR_ARGS("Invalid type!");
            return new NullObject();
        }
    }
    else if(dl) {
        if(ar) {
            return computeDNumber(dl->value, op, float(ar->value));
            
        }
        else if(dr) {
            return computeDNumber(dl->value, op, (dr->value));
        }
        else if(sr) {
            return new ObjectSub<std::string>(dl->toString()+sr->value);
        }
        else {
            PRINT_ERR_ARGS("Invalid type!");
            return new NullObject();
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
            return new NullObject();
        }
    }

    else {
        if(op=="+") {
            qDebug()<<QString::fromStdString(l->toString());
            return new ObjectSub<std::string>(dynamic_cast<ObjectSub<std::string>*>(l)->value+dynamic_cast<ObjectSub<std::string>*>(r)->value);
        }
        else if(op=="==") {
            
            return new ObjectSub<int>(l->toString()==r->toString());
        }
        
        else {
            PRINT_ERR_ARGS("Invalid type!");
            return new NullObject();
        }
    }
}

Object* Operators::eval(Enviroment &env) {
    auto op = child(0);
    auto types = child(1);
    auto params = child(2);
    auto bk = child(3);
    ObjectSub<Function*>* opf=new ObjectSub<Function*>(new Function(dynamic_cast<ParameterList*>(params),dynamic_cast<BlockStmnt*>(bk),env));
    auto csl=dynamic_cast<ObjectSub<ClassInfo*>*>(env.get(types->child(0)->toString()))->value;

    auto csr = dynamic_cast<ObjectSub<ClassInfo*>*>(env.get(types->child(1)->toString()))->value;
    oprt_func[Op_Ci(csl,op->toString(),csr)]=opf;
    return opf;
}

Object* BinaryExpr::eval(Enviroment&env) {
    auto op = oPerator();
    auto afterCastRight = dynamic_cast<Name*>(right());
    auto l = (left())->eval(env);
    auto r = (right())->eval(env);
    auto icl = dynamic_cast<ObjectSub<Instance*>*>(l);
    auto icr = dynamic_cast<ObjectSub<Instance*>*>(r);
    if(icl&&icr) {
        return computeOp(l, op, r);
    }
/*#define shortOperators(sop)\
    if(op == (std::string(#sop)+"=")) {\
        auto afterCastN = dynamic_cast<Name*>(left());\
        if(afterCastN) {\
            auto ir = dynamic_cast<ObjectSub<int>*>(this->right()->eval(env));\
            auto il = dynamic_cast<ObjectSub<int>*>(afterCastN->eval(env));\
            auto dr = dynamic_cast<ObjectSub<float>*>(this->right()->eval(env));\
            auto dl = dynamic_cast<ObjectSub<float>*>(afterCastN->eval(env));\
            if(ir&&il)\
            env.put(afterCastN->name(),  new ObjectSub<int>(il->value sop ir->value));\
            else if(dr&&dl)\
            env.put(afterCastN->name(),  new ObjectSub<float>(dl->value sop dr->value));\
            else\
            int a=3;\
            return afterCastN->eval(env);}}\

    shortOperators(+)
    shortOperators(-)
    shortOperators(*)
    shortOperators(/)
#undef shortOperators*/


   else if((op == "="||op =="+="||op =="-="||op =="*="||op =="/=")) {
        auto rightP = (right())->eval(env);
        return computeAssign(env, rightP);
    }

    else {

        return computeOp(l, op, r);
    }
}

Object* Fun::eval(Enviroment& env){
    return new ObjectSub<Function*>(new Function(parameters(),body(),env));
}

Object* DefStmnt::eval(Enviroment& env){
   auto result= new ObjectSub<Function*>(new Function(parameters(),body(),env));
    env.putNew(name(), result);
    return result;
}

Object* Arguments::eval(Enviroment& callerEnv,Object* value ) {
    auto func = dynamic_cast<ObjectSub<Function*>*>(value);
    if(!func) {
        PRINT_ERR_ARGS("Bad Function!");
        return new NullObject();
    }
    auto params = func->value->parameters();
    if(size()!=params->size()) {
        PRINT_ERR_ARGS("Bad number of arguments!");
        return new NullObject();
    }
    auto newEnv = func->value->makeEnv();
    int num = 0;
    for(auto s=this->children();s!=_children.end();s++) {
        dynamic_cast<ParameterList*>(params)->eval(*newEnv,num++,(*s)->eval(callerEnv));
    }

    Object* result= func->value->body()->eval(*newEnv);
    auto ret = dynamic_cast<ReturnObject*>(result);
    if(ret)
        return ret->rValue;
    else
        return result;
}

Object* FixedArguments::eval(Enviroment &callerEnv, Object *value) {
    auto func = dynamic_cast<NativeFunction*>(value);
    if(!func)
        return Arguments::eval(callerEnv, value);
    if(func->toString()== "print") {
        auto pFunc = dynamic_cast<PrintFunction*>(func);
        if(!pFunc) {
            PRINT_ERR_ARGS("Incorrect print function!");
        }
        QString temp=QString::fromStdString((this->child(0)->eval(callerEnv))->toString());
        pFunc->_output->insert(temp);
        // std::cout<<(this->child(0)->eval(callerEnv))->toString();
        return new ObjectSub<Comment>(Comment("std::cout"));
    }
    else if(func->toString()== "toString") {
        auto pFunc = dynamic_cast<NewInstanceFunction*>(func);
        if(!pFunc) {
            PRINT_ERR_ARGS("Incorrect toString function!");
        }
        QString temp=QString::fromStdString((this->child(0)->eval(callerEnv))->toString());

        // std::cout<<(this->child(0)->eval(callerEnv))->toString();
        return new ObjectSub<std::string>(temp.toStdString());
    }
    else if(func->toString()=="new") {
       auto nFunc = dynamic_cast<NewInstanceFunction*>(func);
       qDebug()<<this->numChildren();
      return (new NewObject(std::vector<ASTree*>{}, nFunc->_ci,this))->eval(callerEnv);
    }
    else if(func->toString() == "time") {
        auto duration = time(nullptr) - ((ObjectSub<long>*)callerEnv.get("startime"))->value;
        return new ObjectSub<long>(duration);
    }
    else if(func->toString()=="Vergo_Global_Package_at") {
        auto tString = (this->child(0)->eval(callerEnv))->toString();
        auto index = ((ObjectSub<int>*)(this->child(1)->eval(callerEnv)))->value;
        return new ObjectSub<std::string>(tString.substr(index,1));

    }
    else if(func->toString()=="Vergo_Global_Package_length") {
        auto tString = (this->child(0)->eval(callerEnv))->toString();
        //auto index = ((ObjectSub<int>*)(this->child(1)->eval(callerEnv)))->value;
        return new ObjectSub<int>(tString.length());

    }
    else if(func->toString()=="Vergo_Global_Package_newFstream") {
       // auto fString = (this->child(0)->eval(callerEnv))->toString();
        std::fstream* fptr = new std::fstream();
        return new FstreamObj(fptr);
    }
    else if(func->toString()=="Vergo_Global_Package_open") {
        auto fString = (this->child(0)->eval(callerEnv))->toString();
        auto fs = dynamic_cast<FstreamObj*>(this->child(1)->eval(callerEnv))->_fs;
       fs->open("/Users/herangao/build-FileTester-Desktop_Qt_5_9_1_clang_64bit-Debug/"+fString,std::fstream::in | std::fstream::out | std::fstream::app);
       return new TrueObject;
    }
    else if(func->toString()=="Vergo_Global_Package_close") {
       // auto fString = (this->child(0)->eval(callerEnv))->toString();
        auto fs = dynamic_cast<FstreamObj*>(this->child(0)->eval(callerEnv))->_fs;
       fs->close();
       return new TrueObject;
    }
    else if(func->toString()=="Vergo_Global_Package_input") {
        auto fString = (this->child(0)->eval(callerEnv))->toString();
        auto fs = dynamic_cast<FstreamObj*>(this->child(1)->eval(callerEnv))->_fs;
        *fs << fString;
        return new TrueObject;
    }
    else if(func->toString() == "scanf") {
        auto pFunc = dynamic_cast<ScanfFunction*>(func);
        if(!pFunc) {
            PRINT_ERR_ARGS("Incorrect scanf function!");
            return new NullObject();
        }
        QString qinput=pFunc->_output->text();
        qDebug()<<qinput;
        pFunc->_output->setReadOnly(false);
        pFunc->_timer->exec();
          qDebug()<<QString::fromStdString(pFunc->afterInput);
        qinput = QString::fromStdString(pFunc->afterInput).mid(qinput.length());
        qDebug()<<qinput;
        std::string input = qinput.toStdString();
        auto castName = dynamic_cast<Name*>(child(0));
        if(!castName) {
            PRINT_ERR_ARGS("Bad Name type!");
            return new NullObject();
        }
        if(input.at(0)=='"') {
            input = input.substr(1,input.length()-2);
            auto re =  new ObjectSub<std::string>(input);
            callerEnv.put(castName->name(),re);
            return re;
        }
        else if(QString::fromStdString(input).indexOf(".")==-1){
            qDebug()<<QString("it");
            auto re = new ObjectSub<int>(std::stoi(input));
            callerEnv.put(castName->name(),re);
            return re;
        }
        else {
            qDebug()<<QString("it");
            auto re = new ObjectSub<double>(std::stod(input));
            callerEnv.put(castName->name(),re);
            return re;
        }
    }
    else {
        PRINT_ERR_ARGS("Neither User-defined function nor local function!");
        return new NullObject();
    }
    
}


Object* ClassStmnt::eval(Enviroment &env) {
    auto v = new ObjectSub<ClassInfo*>(new ClassInfo(this,&env));
    if(!v){
        PRINT_ERR;
        auto nobj = new NullObject();
        env.put(name(),nobj);
        return nobj;
    }

    env.put(name(),v);
   // return new ObjectSub<std::string>(name());
    return v;
}

Object* ClassBody::eval(Enviroment &env) {
    for(auto t:_children) {
        t->eval(env);
    }
    return new NullObject();
}

Object* Dot::eval(Enviroment &env,Object* value) {
    auto member = name();
    if("new" == member) {
        //auto ci = dynamic_cast<ObjectSub<ClassInfo*>*>(value);
        return new NewInstanceFunction("new",value);
    }
     auto ci = dynamic_cast<ObjectSub<ClassInfo*>*>(value);
     if(ci) {
         auto astfunc =ci->value->findFunction(member);
         auto def = dynamic_cast<DefStmnt*>(astfunc);
         if(!def) {
             PRINT_ERR_ARGS("Bad def!");
             return new NullObject();
         }
         else {
             return def->Fun::eval(env);
         }
     }

    auto ins = dynamic_cast<ObjectSub<Instance*>*>(value);
    if(!ins) {
        PRINT_ERR_ARGS("Bad member access!");
        return new NullObject();
    }

    else {
        return ins->value->read(member);
    }
}

Object* NewObject::eval(Enviroment &env) {
    if(numChildren()!=0) {
         _ciobj = child(0)->eval(env);
    }
    auto classInformObj =dynamic_cast<ObjectSub<ClassInfo*>*>(_ciobj);
    if(!classInformObj) {
        PRINT_ERR_ARGS("can't find the given ClassInfo!");
        return new NullObject();
    }
    else {
        Enviroment* nenv = new NestedEnv(classInformObj->value->enviroment());
        ClassInfo* sc = classInformObj->value;
        ClassInfo::initObj(sc, nenv,_p);
        auto newObj = new ObjectSub<Instance*>(new Instance(nenv,sc));
        newObj->value->putMember("this", newObj);
        return newObj;
    }
}

