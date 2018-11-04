//
//  Object.h
//  Stone
//
//  Created by Heran Gao on 6/29/17.
//  Copyright © 2017 Heran.Gao. All rights reserved.
//

#ifndef Object_h
#define Object_h
#include <string>
#include <iostream>
#include<map>
#include<type_traits>
#include<fstream>
class Comment
{
public:
    Comment(std::string com):comment(com) {}
    std::string toString() {
        return comment;
    }
private:
    std::string comment;
};



class Object
{
public:
    virtual std::string toString() = 0;
    
};


class TrueObject:public Object
{
public:

     virtual std::string toString() override {
        return "true";
    }
};

class FalseObject:public Object
{
public:

     virtual std::string toString() override {
        return "false";
    }
};


class ReturnObject:public Object
{
public:
    ReturnObject(Object* rv):rValue(rv){}
    Object* rValue=nullptr;
     virtual std::string toString() override {
        return rValue->toString();
    }
};

class BreakObject:public Object {
public:
    virtual std::string toString() override {
        return "Break";
    }
};

class NullObject:public Object {
public:
    virtual std::string toString() override {
        return "null";
    }
};

class FstreamObj:public Object
{
public:
    FstreamObj(std::fstream* fs):_fs(fs){}
    virtual std::string toString() override {
        return "FstreamObj";
    }
    std::fstream* _fs;
};

template<typename T>
class ObjectSub:public Object
{
public:
    ObjectSub<T>(const T& v):value(v){}
    T value;
    virtual std::string toString() override{
        return toStrHelper(0);
    }
private:
    template<typename N>
    std::string toStringIfPointer(N* _value) {
        return _value->toString();
    }
    template<typename N>
    std::string toStringIfPointer(N _value) {
        return _value.toString();
    }
    template<typename C=T>
    auto toStrHelper(int )->decltype(std::declval<typename std::remove_pointer<C>::type>().toString())  {
        return //value->toString();
                toStringIfPointer(value);

    }
    template<typename C=T>
    auto toStrHelper(char)->decltype(std::to_string(std::declval<C>())) {
        return std::to_string(value);
    }
    template<typename C=T>
    std::string toStrHelper(...) {
        return std::string(typeid(C).name())+"\n";
    }
   };

template<>
class ObjectSub<std::string> :public Object
{
public:
    std::string value;
    ObjectSub<std::string>(std::string v):value(v){}
    ObjectSub<std::string>& operator = (std::string v){
        value = v;
        return *this;
    }
    virtual std::string toString() override {
        return value;
    }
    
};


class ArrayWrapper : public Object
{
public:
    //Object** array;
    /*ArrayWrapper(Object** const & a,int num) : array(a),mNum(num) {
        
    }*/
    ArrayWrapper() {

        }
    int memberNumber() {
        return exArray.size();
    }
    Object* atInt(int index) {
        auto result = exArray[index];
        if(!result) {
            return new NullObject();
        }
        return result;
    }
    Object* atStr(std::string index) {
        auto result = acArray[index];
        if(!result) {
            return new NullObject();
        }
        return result;
    }

    virtual std::string toString() override {
        return "ArrayWrapper: array[0] is ";
                //+(*array)->toString();
    }
     std::map<std::string,Object*> acArray;
     std::map<int,Object*> exArray;
private:
    int mNum=0;

};


/*std::string operator - ( std::string l, int r)  {
    return  l + std::to_string(r);
}*/


/*
ObjectSub<int> operator +( ObjectSub<int>& l, const ObjectSub<int>& r)  {
    return  l.value + r.value;

}
ObjectSub<int> operator -( ObjectSub<int>& l, const ObjectSub<int>& r)  {
   return  l.value - r.value;
}
ObjectSub<int> operator *( ObjectSub<int>& l, const ObjectSub<int>& r)  {
   return  l.value * r.value;
}
ObjectSub<int> operator /( ObjectSub<int>& l, const ObjectSub<int>& r)  {
   return  l.value + r.value;
}

ObjectSub<float>& operator +(const ObjectSub<int>& l,  ObjectSub<float>& r)  {
    r.value += l.value;
    return r;
}
ObjectSub<float>& operator -=(const ObjectSub<int>& l,  ObjectSub<float>& r)  {
    r.value -= l.value;
    return r;
}
ObjectSub<float>& operator *=(const ObjectSub<int>& l,  ObjectSub<float>& r)  {
    r.value *= l.value;
    return r;
}
ObjectSub<float>& operator /=(const ObjectSub<int>& l,  ObjectSub<float>& r)  {
    r.value /= l.value;
    return r;
}

ObjectSub<float>& operator +=( ObjectSub<float>& l, const ObjectSub<int>& r)  {
    l.value += r.value;
    return l;
}
ObjectSub<float>& operator -=( ObjectSub<float>& l, const ObjectSub<int>& r)  {
    l.value -= r.value;
    return l;
}
ObjectSub<float>& operator *=( ObjectSub<float>& l, const ObjectSub<int>& r)  {
    l.value *= r.value;
    return l;
}
ObjectSub<float>& operator /=( ObjectSub<float>& l, const ObjectSub<int>& r)  {
    l.value /= r.value;
    return l;
}

ObjectSub<float>& operator +=( ObjectSub<float>& l, const  ObjectSub<float>& r)  {
    l.value += r.value;
    return l;
}
ObjectSub<float>& operator -=( ObjectSub<float>& l, const  ObjectSub<float>& r)  {
    l.value -= r.value;
    return l;
}
ObjectSub<float>& operator *=( ObjectSub<float>& l, const  ObjectSub<float>& r)  {
    l.value *= r.value;
    return l;
}
ObjectSub<float>& operator /=( ObjectSub<float>& l, const  ObjectSub<float>& r)  {
    l.value /= r.value;
    return l;
}

ObjectSub<std::string>& operator +=( ObjectSub<std::string>& l,  const ObjectSub<std::string>& r)  {
    l.value += r.value;
    return l;
}
ObjectSub<std::string>& operator +=( ObjectSub<std::string>& l,  const ObjectSub<int>& r)  {
    l.value += std::to_string(r.value);
    return l;
}
ObjectSub<std::string>& operator +=( ObjectSub<std::string>& l,  const ObjectSub<float>& r)  {
    l.value += std::to_string(r.value);
    return l;
}
ObjectSub<std::string>& operator +=(const ObjectSub<int>& l,   ObjectSub<std::string>& r)  {
    r.value += std::to_string(l.value);
    return r;
}
ObjectSub<std::string>& operator +=(const ObjectSub<float>& l,   ObjectSub<std::string>& r)  {
    r.value += std::to_string(l.value);
    return r;
}


*/


/*
 template<typename T>
 class Test
 {
 public:
 
 template<typename C=T>
 auto toString(int )->decltype(std::declval<C>().toString())  {
 return "yes";
 }
 template<typename C=T>
 auto toString(char)->decltype(std::to_string(std::declval<C>())) {
 return "ok";
 }
 template<typename C=int>
 std::string toString(...) {
 return "no";
 }
 std::string to_string() {
 return toString(0);
 }
 };

 class Object
{
public:
    virtual std::string toString() = 0;
    
};


template<typename T>
class ObjectSub : public Object{
public:
    ObjectSub(std::string cm):comment(cm){}
    ObjectSub(){}
    virtual std::string toString()override {
        return comment+" "+typeid(T).name()+"\n";
    }
private:
    std::string comment;

};

template<>
class ObjectSub<int> :public Object
{
public:
    int value=0;
    ObjectSub<int>(int v):value(v){}
    ObjectSub<int>& operator = (int v){
        value = v;
        return *this;
    }
    virtual std::string toString() override {
        return std::to_string(value);
    }
    
};

template<>
class ObjectSub<long> :public Object
{
public:
    long value=0;
    ObjectSub<long>(long v):value(v){}
    ObjectSub<long>& operator = (long v){
        value = v;
        return *this;
    }
    virtual std::string toString() override {
        return std::to_string(value);
    }
    
};

*/



#endif /* Object_h */
