#ifndef CODEEDITOR_H
#define CODEEDITOR_H
#include<QMainWindow>
#include<QTextEdit>
#include<QAction>
#include "highlighter.h"
#include<QPair>
#include"hint.h"
class CodeEditor :public QTextEdit
{
    Q_OBJECT
public:
    explicit CodeEditor(QMainWindow* parent);
    void setTag(QString tag) {
        _tag = tag;
    }
    QString getTag() {
        return _tag;
    }
    QFile file{"/Users/herangao/build-FileTester-Desktop_Qt_5_9_1_clang_64bit-Debug/homa.txt"};

private:
    QMainWindow* _mw;
    std::string passage;
   public:
    bool ifReturnPressed=false;
    bool ifRBrace=false;

    QString indentation{""};
    struct cursonPos {
        int del;
        int current;
        bool ifDeletePressed=false;
        int rBracePos=0;
    }cp;
    struct BraceCount {
        int left=0;
        int right=0;
        int diff() {
            return left-right;
        }
    }bc;
 private:


     Highlighter *highlighter;


     template<typename T>
     bool matchLeft(QTextBlock currentBlock, int i, int numLeftParentheses,char l, char r,T n);
      template<typename T>
     bool matchRight(QTextBlock currentBlock, int i, int numRightParentheses,char l, char r,T n);
     void createParenthesisSelection(int pos);
     void matchParentheses();


      void countBraces();



     void setHintOn(bool i);

     Hint* ht = nullptr;
     QString _tag;


 protected:



};

class EditorInterface
{
public:


    CodeEditor* displayedCE() {
        return displayed;
    }
    CodeEditor* createNew(QMainWindow* parent,QString tag) {
        auto nEditor = new CodeEditor(parent);
        nEditor->setTag(tag);
        editors.append(nEditor);
        nEditor->move(120,0);
        nEditor->setFixedSize(481,251);
        setNewDisplay(nEditor);
        nEditor->file.setFileName(tag);
        return nEditor;
    }
    CodeEditor* setNewDisplay(CodeEditor* n) {
        if(displayed)
        displayed->setVisible(false);
        displayed=n;
        n->setVisible(true);
        return n;
    }
    QList<CodeEditor*> editors;

private:

    CodeEditor* displayed=nullptr;

};

#endif // CODEEDITOR_H
