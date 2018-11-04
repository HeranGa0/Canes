#include "BasicParser.h"
#include<QDebug>
void BasicParser::token(std::string name)
{
    Token* t = lexer.read();
    bool s =!(t->isIdentifier() && name == t->getText());
    if (s) {
        PRINT_ERR_ARGS("next token is NOT "+name);
    }
}

bool BasicParser::isToken(std::string name)
{
    Token* t = lexer.peek(0);
    return t->isIdentifier() && name == t->getText();
}

ASTree* BasicParser::param()
{
    if(lexer.peek()->isIdentifier())
        return new ASTLeaf(lexer.read());
    else {
        PRINT_ERR_ARGS("next token is NOT a Identifier!");
        return nullptr;
    }
}

ASTree* BasicParser::params()
{
    std::vector<ASTree*> list;
    
    auto _param = param();
    list.push_back(_param);
    while(isToken(",")) {
        token(",");
        auto p = param();
        list.push_back(p);
    }
    return new ParameterList(list);
}

ASTree* BasicParser::param_list()
{   ASTree* _params = nullptr;
    if(isToken("(")) {
        token("(");
        skipEOL();
        if(lexer.peek()->isIdentifier()&&(!isToken(")"))) {
             _params = params();
        }
        token(")");
    }
    else {
        PRINT_ERR_ARGS("Token should be '('");
    }
    if(_params)
        return _params;
    else
        return new ParameterList(std::vector<ASTree*> {});
}

ASTree* BasicParser::def() {
    std::vector<ASTree*> list;
    if(isToken("fun")) {
        token("fun");
        skipEOL();
        if(lexer.peek()->isIdentifier()) {
            auto idName = lexer.read();
            list.push_back(new ASTLeaf(idName));
            skipEOL();
            auto plist = param_list();
            list.push_back(plist);
            skipEOL();
            auto bk = block();
            list.push_back(bk);
        }
        else {
            PRINT_ERR_ARGS("next token should be a Identifier!");
        }
    }
    else {
        PRINT_ERR_ARGS("next token should be 'fun'");
    }
    return new DefStmnt(list);
}

ASTree* BasicParser::args() {
    std::vector<ASTree*> list;
    auto expr = expression();
    list.push_back(expr);
    while(isToken(",")) {
        token(",");
        list.push_back(expression());
    }
    return new FixedArguments(list);
    
}

ASTree* BasicParser::postfix()
{
    if(isToken(".")) {
        token(".");
        auto ider = new ASTLeaf(lexer.read());
        std::vector<ASTree*> lt{ider};
        return new Dot(lt);
        //return nullptr; //delete later
    }
    else if(isToken("[")) {
        token("[");
        auto arrayIndex = expression();
        token("]");
        std::vector<ASTree*> ls{arrayIndex};
        return new ArraySuffix(ls);
    }
    ASTree* arg = nullptr;
    if(isToken("(")) {
        token("(");
        if(!isToken(")")) {
            arg = args();
        }
        if(!isToken(")")) {
            PRINT_ERR_ARGS("Token should be ')'");
        }
        token(")");
    }
    else {
        PRINT_ERR_ARGS("Token should be '('");
    }
    if(arg)
        return arg;
    else {
        std::vector<ASTree*> temp{};
        return new FixedArguments(temp);
    }
}

ASTree* BasicParser::createInstance()
{
    token("new");
    std::vector<ASTree*> list{expression()};
    return new NewObject(list);
}

ASTree * BasicParser::primary()
{
    ASTree* result = nullptr;
    if (isToken("(")) {
        token("(");
         result = expression();
        token(")");
        
        }
    else if(isToken("new")) {
        result = createInstance();
    }
    else if(isToken("break")) {
        result = new BreakStmnt(lexer.read());
    }
    else if(isToken("return")) {
        token("return");
        result = new ReturnStmnt(std::vector<ASTree*>{expression()});
    }
    else if(isToken("fun")) {
        token("fun");
        skipEOL();
        auto pas = param_list();
        skipEOL();
        auto bk = block();
        auto vec =std::vector<ASTree*>{pas,bk};
        result = new Fun(vec);
    }
    else if(isToken("[")) {
        token("[");
        result = elements();
        token("]");
    }
    else {
        auto ntoken = lexer.read();
        if (ntoken->isNumber()) {
           result = new NumberLiteral(ntoken);
            
        }
        else if (ntoken->isIdentifier()) {
            if(ntoken->getText()=="nil") {
                result= new NilLiteral(ntoken);
            }
            else if(ntoken->getText()=="true") {
                result= new Trueiteral(ntoken);
            }
            else if(ntoken->getText()=="false") {
                result= new Falseiteral(ntoken);
                qDebug()<<QString::fromStdString(lexer.peek(0)->getText());
            }
            else
            result = new Name(ntoken);
        }

        else if (ntoken->isString()) {
            result = new StringLiteral(ntoken);
        }
        else if(ntoken->isDnumber()) {
            result = new DnumberLiteral(ntoken);
        }
        else {
            std::cerr << __FILE__ << __LINE__ << std::endl;
            return nullptr;
        }
    }
    std::vector<ASTree*> list;
    list.push_back(result);
    while(isToken("(")||isToken(".")||isToken("[")) {
        list.push_back(postfix());
    }
    return PrimaryExpr::create(list);
}



ASTree * BasicParser::factor()
{
    if (lexer.peek(0)->getText() == "-") {
        auto ntoken = lexer.read();
        auto prime = primary();
        return new NegativeExpr(std::vector<ASTree*>{new ASTLeaf(ntoken), prime});
    }
    else if (lexer.peek(0)->getText() == "!") {
        auto ntoken = lexer.read();
        qDebug()<<QString::fromStdString(ntoken->getText());
        auto prime = primary();
        return new NotExpr(std::vector<ASTree*>{new ASTLeaf(ntoken), prime});
    }
    else {
        auto prime = primary();
        return prime;
    }
}


ASTree * BasicParser::expression()
{
    auto right = factor();
    Precedence* prec;
    while ((prec = nextOperator())) {
        right = doShift(right, prec->getValue());
    }
    return right;
}

ASTree* BasicParser::block()
{   std::vector<ASTree*> bList;
    auto pToken = lexer.peek(0);
    qDebug()<<QString::fromStdString(pToken->getText());
    if(pToken->getText()=="{") {
        token("{");
        auto sNode = statement();
        if(sNode) {
            bList.push_back(sNode);
        }
        qDebug()<<QString::fromStdString(sNode->toString());
        while(!isToken("}")) {
        while(isToken(";")||isToken(Token::EOL)){
              //||isToken("}")) {
            //if(!isToken("}")) {
            auto nToken = lexer.read();}
            //qDebug()<<QString::fromStdString(nToken->getText());
            if(!(isToken("}")||isToken(";")||isToken(Token::EOL))) {
                auto nNode = statement();
                 qDebug()<<QString::fromStdString(nNode->toString());
                if(nNode) {
                    bList.push_back(nNode);
                }
            }//}
        }
        
        

        auto rToken = lexer.peek();
        qDebug()<<QString::fromStdString(rToken->getText());
        if(rToken->getText()=="}") {

            token("}");
            return new BlockStmnt(bList);
            
        }
        else {
            std::cerr<<__FILE__<<__LINE__<<std::endl;
            return nullptr;
        }
    }
    else {
        std::cerr<<__FILE__<<__LINE__<<std::endl;
        return nullptr;
    }
}

ASTree* BasicParser::statement() {
   while(isToken("\\n")) {
        token("\\n");
    }

    
    if(isToken("if")) {
        token("if");
        qDebug()<< QString::fromStdString(lexer.peek(0)->getText());
        auto eNode = expression();
        qDebug()<< QString::fromStdString(lexer.peek(0)->getText());
        skipEOL();
        auto bNode = block();
         qDebug()<<QString::fromStdString(bNode->toString());
        skipEOL();
        std::vector<ASTree*> list{eNode,bNode};
        
        ASTree* elseNode = nullptr;
        while(isToken("orif")) {
            token("orif");
            list.push_back(expression());
            skipEOL();
            list.push_back(block());
            skipEOL();
        }
        if(isToken("else")) {
            token("else");
            skipEOL();
            elseNode = block();
            list.push_back(elseNode);
        }
        return new IfStmnt(list);
        
    }
    else if(isToken("while")) {
        token("while");
        auto eNode = expression();
        skipEOL();
        auto bNode = block();
        return new WhileStmnt(std::vector<ASTree*>{eNode,bNode});
    }
    else if(isToken("for")) {
        token("for");
        token("(");
        auto iNode = expression();
        if(isToken(":")) {
            token(":");
            auto aNode = expression();
            token(")");
            skipEOL();
            auto bNode = block();
            return new Foreach(std::vector<ASTree*>{ iNode,aNode,bNode});
        }
        token(";");
        auto cNode = expression();
        token(";");
        auto itNode = expression();
        token(")");
        skipEOL();
        auto bNode = block();
        return new ForStmnt(std::vector<ASTree*>{iNode,cNode,itNode,bNode});
    }
    else {
        return expression();
    }
}


ASTree* BasicParser::member() {
    auto nToken = lexer.peek();
    if(nToken->getText() == "fun") {
        return def();
    }
    return expression();
}

ASTree* BasicParser::class_body() {
    std::vector<ASTree*> lt;
    skipEOL();
    token("{");
    /*if(!(isToken(";")||isToken(Token::EOL)||isToken("}"))) {
        lt.push_back(member()) ;
    }
    while(isToken("}")||) {
        lexer.read();
        if(!isToken("}")) {
           lt.push_back(member());
        }
    }*/
    while(isToken(";")||isToken(Token::EOL)) {
        auto nToken = lexer.read();
        if(!(isToken("}")||isToken(";")||isToken(Token::EOL))) {
            auto nNode = member();
            if(nNode)
                lt.push_back(nNode);
            else
                PRINT_ERR_ARGS("Bad member!");
        }
    }
    token("}");
    return new ClassBody(lt);
}

ASTree* BasicParser::defclass() {
    token("class");
    std::vector<ASTree*> list;
    auto ider = new ASTLeaf(lexer.read());
    list.push_back(ider);
    if(isToken("extends")) {
        token("extends");
        list.push_back(new ASTLeaf(lexer.read()));
    }
    list.push_back(class_body());
    return new ClassStmnt(list);
}

ASTree* BasicParser::typelist(){
    token("(");
    std::vector<ASTree*> list;
    while(isToken(Token::EOL)) {
        lexer.read();
    }
    do {
         auto x= expression();
        list.push_back(x);
        qDebug()<<QString::fromStdString(x->toString());
        while(isToken(",")||isToken(Token::EOL)) {
           lexer.read();

        }
    }
    while(!isToken(")"));
    token(")");
    return new ParameterList(list);
}

ASTree* BasicParser::elements() {
    std::vector<ASTree*> list;
    while(isToken(Token::EOL)) {
        lexer.read();
    }
   // list.push_back(expression());
    while(!isToken("]"))
    {
        list.push_back(expression());
        while(isToken(",")||isToken(Token::EOL)) {
            lexer.read();

        }
    }

   /* while(!isToken("]")) {
        while(isToken(",")||isToken(Token::EOL)) {
            lexer.read();

        }
         list.push_back(expression());
    }*/
    return new ArrayRV(list);
}

ASTree* BasicParser::operatorfunc() {
    token("operator");
    skipEOL();
    if(!lexer.peek()->isIdentifier())
        PRINT_ERR_ARGS("is not identifier!");
    auto op = lexer.read();
    skipEOL();
    auto typel = typelist();
    skipEOL();
    auto parames = param_list();
    skipEOL();
    auto oBody = block();
    return new Operators(std::vector<ASTree*>{new ASTLeaf(op),typel,parames,oBody});
}

ASTree* BasicParser::program() {
    while(isToken(";")||isToken(Token::EOL)) {
        lexer.read();
    }
    ASTree* nNode = nullptr;
    if(isToken("fun")) {
        nNode = def();
    }
    else if(isToken("class")) {
        nNode = defclass();
    }
    else if(isToken("operator")) {
        nNode = operatorfunc();
    }
    else {
        nNode = statement();
    }
    while(isToken(";")||isToken(Token::EOL)) {
        lexer.read();
    }
    return nNode;
}



ASTree * BasicParser::doShift(ASTree * left, int prec)
{
    auto op = new ASTLeaf(lexer.read());
    Precedence* nextOp;
    auto right = factor();
    while ((nextOp = nextOperator()) && rightIsExpr(prec, nextOp)) {
        right = doShift(right, nextOp->getValue());
    }
    return new BinaryExpr(std::vector<ASTree*>{left, op, right});
}

BasicParser::Precedence* BasicParser::nextOperator()
{
    auto token = lexer.peek(0);
    auto iterator = operators.find(token->getText());
    //auto t1 = iterator->first;
    // bool t2 = token->isIdentifier();
    //std::cout<<t1<<t2<<std::endl;
    if (token->isIdentifier() && iterator != operators.end())
        return iterator->second;
    else return nullptr;
}

bool BasicParser::rightIsExpr(int prec, Precedence * nextPrec)
{
    if (nextPrec)
        return prec < nextPrec->getValue();
    else
        return false;
}




