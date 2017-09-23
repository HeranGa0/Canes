#include"Lexer.h"
#include"ExprParser.h"
#include"OpPrecedenceParser.h"
#include"BasicParser.h"
#include <time.h>
///*
int main() {
    const auto startTime = time(nullptr);
    
    BasicParser p("text.txt");
    NestedEnv env;
    env.put("print", new NativeFunction("print"));
    env.put("time", new NativeFunction("time"));
    env.put("scanf", new NativeFunction("scanf"));
    env.put("startime", new ObjectSub<long>(startTime));
    Object* result;
       while(!p.isEndOfFile()) {
        auto node = p.program();
           result =node->eval(env);
    }
   // std::cout<<result->toString()<<std::endl;
	//OpPrecedenceParser p("text2.txt");

	//auto r = p.expression();
	//std::cout << "=>" << r->toString();
    
	/*for (Token* t; (t = l.read()) != &Token::EOF_TOKEN;) {
		int a = 1;
		std::cout << "=>" + t->getText()<<std::endl;
	}*/
    return 0;
}
//*/
