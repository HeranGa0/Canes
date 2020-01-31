# Canes
<a href="https://drive.google.com/open?id=15qoyqwHrMZn9yYXWPpLnoVsamEQE3u_q"> Download Canes Editor and test immediately on Mac here! </a> (Notice that occasionally the editor crushes when compiling the incorrect scripts, however the bug will be fixed soon.)

## What is Canes?
Canes is a scripting language designed by me and implemented based on C++. Its syntax is similar to python or javascript, or the combinaiton of both. Anyone who is familar with each can learn Canes in an hour. Although its syntax is not complicated, it has its own uniqness and can actually do many cool things. 
So why would I design it? The anwser is simple, just for fun. Designing a programming language and writing it an interpreter is such a romantic thing for our programmers, isn't it? The other point is that, altohough Canes might not have many advanages over javascript and python, it has its own charm. Don't believe it? You can read the following handbook and try to code a little bit in a simple editor for Canes that I made. Have fun! 

## Learn Canes in Ten Minutes
```python
print("Hello,world") 
```
commenting in Canes:
```c
// for one line comment
/* for 
multiple
lines
Comments
*/
```
Using Canes as a Calculator:
Numbers:
Expression syntax for numbers is straightforward: the operators +, -, * and / work just the same in most programming languages (e.g. c,python); parentheses () are used for grouping. 
For example:
```c++
2 + 3 //5
50 - 2*5 // 40
(50 - 2*5) // 40
If both numbers are integers, the result will also be integer:
8 / 5   // 1; 
If at least one number is float, then the result will be float:
8.0 / 5  //1.6 
the % operator returns the remainder of the division:
14 % 5  //4;
```
The integer numbers (e.g. 1, 2, 100,10000) have type int, the ones with a fractional part (e.g. 1.0, 2.5,100.125) have type float. 
The equal sign (=) is used to assign a value to a variable.
```c++
var1 = 4 
print(var1) //4
var2 = 3.5
print(var2) //3.5
var3 = var1+var2
print(var3) // 7.5
```

Booleans:
Booleans can only have two values: true or false.
```c++
var1 = true
print(var1) // true
var2 = false
print(var2) //false
More about booleans will discuss  later
```

String:
Just like we did with numbers, we are declaring a variable, initializing it with a string value, and then returning the value. The only difference here is that when writing a string, you need to surround the value with double quotes.
```c++
 str1 = "This is a string";
 print(str1) //This is a string
 str2 = str1
 print(str2) //This is a string
 ```
 Concatenating strings:
```c++
 first = "How ";
 second = "are you?";
 joined = one + two;
 print(joined)// How are you?
 ```
 Concatenating strings with numbers:
 ```c++
 first  = "I am"
 second = 27
 third = " years old"
 result = first + second + third
 print(result) // I am 27 years old
 ```
Operators:
Assign values to variables and add them together:
```c++
x = 1;         // assign the value 1 to x
y = 2;         // assign the value 2 to y
z = x + y;     // assign the value 3 to z, which equals to (x + y)
```
```c++
Assign shortcuts:
x += y	//same as x = x+y
x -= y //same as x = x-y
x *= y	//same as x = x*y
x /= y	//same as x = x/y
x %= y//same as x = x%y
```

==	equal to
!=	not equal
>	greater than
<	less than
>=	greater than or equal to
<=	less than or equal to

List:
Canes lists are used to store multiple values in a single variable.
The elements in the list can be both numbers and string,iterals.
Creating an list:
```javascript
var list_name = [item1, item2, ...];   
```
Example:
```javascript
var cars = ["red", 1, "blue"];
```
Access the Elements of a list
You access a list element by referring to the index number or the index string
```javascript
li = ["red", 1, "blue"];
print(li[0]) //red
li["mColor"] = "black"
print(li["mColor"]) // "black"
```
Canes lists are first-class values ([What are first-class values](http://lambda-the-ultimate.org/node/1518)）

Conditional Statements:
In Canes we have the following conditional statements:
Use if to specify a block of code to be executed, if a specified condition is true
Use else to specify a block of code to be executed, if the same condition is false
Use else if to specify a new condition to test, if the first condition is false
Use switch to specify many alternative blocks of code to be executed
Very similar to c/c++, java and js
```c++
if (condition1) {
    block of code to be executed if condition1 is true
} else if (condition2) {
    block of code to be executed if the condition1 is false and condition2 is true
} else {
    block of code to be executed if the condition1 is false and condition2 is false
}
```
Example:
```c++
i=10
if (i < 10) {
    print("too small");
} else if (i > 3) {
     print("too big");
} else {
     print("exactly!");
}
//exactly!
```
Loops can execute a block of code as long as a specified condition is true:
The While Loop
The while loop loops through a block of code as long as a specified condition is true.
Syntax:
```c++
while (condition) {
    code block to be executed
}
```
While loop Example:
```c++
i=0
while (i < 10) {
    i+=1;
}
print(i) //10
```
The for loop:
Canes  supports two different kinds of loops: 
* loops through a block of code a number of times
* loops through all elements of an list
The first kind of loop has the following syntax:
```c++
for (statement 1; statement 2; statement 3) {
    code block to be executed
}
For example:
for (i = 3; i < 6; i++) {
    print(i)
}
//345
```
The second kind of loop is like the following:
```c++
unsor_list = [1,2,3,4,5]
for (x : unsor_list) {
    text += person[x]
    print(text)
}
//12345
```
Function:
function syntax:
```c++
fun name(parameter1, parameter2, parameter3) {
    code to be executed
}
```
Example
```c++
fun calcplus(num1, num2) {
    return num1+num2       
    //Canes function can only return a single value
}
//call this function with 1,2 as parameters
print(calcplus(1,2)) // 3
```
Since function is first-class value ([What are first-class values](http://lambda-the-ultimate.org/node/1518)) , its definition can be used/passed as a variable, for example:
```c++
fun functor(func1) {
     return func1(1,2)
}
fun add(num1,num2) {
     return num1+num2
}
fun multiply(num1,num2) {
     return num1*num2
}
functor(add) //passing add as a variable to functor, giving a result of 3
functor(multiply) // 2
```
Canes function supports Closures ([What are closures](https://www.lua.org/pil/6.1.html))
Example:
```c++
fun closure(x) {
    return fun() {
         return x = x+1
    }
}
count = closure(0)
count() //1
count() //2
count() //3
```

Class Definition Syntax
The simplest form of class definition looks like this:
```c++
class ClassName {
    <statement-1>
    .
    .
    .
    <statement-N>
}
Example:
class myClass {
    A simple example class
    i = null

    fun f(self) {
        return i
    }
}
```
Here, i is a elemental attribute of myClass, and f is a functional attribute of myClass. All elemental attributes in one class can be assigned and referenced in all functional attributes of that class. 

Class instantiation:
Class instantiation uses the keyword new and create an instantiation of that class. For example (assuming the above class):
```c++
x = myClass.new()
```
Here, it creates a new instance of the class and assigns this object to the variable x.

The instantiation operation creates an empty object. Many classes like to create objects with instances customized to a specific initial state. Therefore a class may define a consturctor function, whose name must be the same to the class name
Example:
```c++ 
class myClass {
    //A simple example class
    i = null
    //constructor myClass
    fun myClass(x) {
        i = x
    }
    fun getI() {
        return i
    }
}
```
When a class defines a constructor method, class instantiation automatically invokes it for the newly-created class instance. So in this example, a new, initialized instance can be obtained by:
```c++
y = myClass.new(2)   
```
dot . operator is used to reference attributes of an object, for example, (assuming the above object y):
```c++
i2 = y.i  //2
i3 = y.getI()  //2
```
Class identified name is also a first-class variable ([What are first-class values](http://lambda-the-ultimate.org/node/1518)), therefore:
```c++
z = myClass   // z is assigned bt myClass
```
Class Inheritance
Syntax:
```java
class DerivedClassName extends BaseClassName {
    //DerivedClass constructor
    func DerivedClassName(arg1,arg2...agrx) {
    //BaseClass constructor
        BaseClassName(agr1)
    }
    <statement-1>
    .
    .
    .
    <statement-N>
 }
 ```
 The first line inside the Derived class constructor must always be base class constructor.
 Once a Derived class has been created, it can access and call all the variables and methods in the base class. For example,
```java
class father
{
       age
       fun father(age1) {
              age = age1
         }     
       fun printAge() {
           print(age)
       }
}

class child extends father {
       height
       fun child(age1) {
              father(age1)
              height1 = 185
            
         }     
}
c1 = child.new(30)
print(c1.age) //30
c1.printAge() //30
```
 
Operator overloading
Syntax:
```c++
operator (Type_left,Type_right)(left,right) {
     statements...
}
```
First key word "operator", the parameters inside the first parentheses are the class types, the parameters inside the second parentheses are the instanced objects of the corresponding two types. For example,
```c++
class UserNumber
{
    i =0
}
operator (UserNumber,UserNumber)(a,b) { //a is the instanced object of UserNumber, b is also the instanced object of UserNumber)
    return a.i+b.i //0
}
```

    


