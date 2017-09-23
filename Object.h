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
    template<typename C=T>
    auto toStrHelper(int )->decltype(std::declval<std::remove_pointer<C>::type>().toString())  {
        return std::remove_pointer<C>::type::toString();
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
    Object** array;
    ArrayWrapper(Object** const & a,int num) : array(a),mNum(num) {
        
    }
    int memberNumber() {
        return mNum;
    }
    virtual std::string toString() override {
        return "ArrayWrapper: array[0] is "+(*array)->toString();
    }
private:
    int mNum=0;
};
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
