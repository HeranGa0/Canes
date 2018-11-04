#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QPlainTextEdit>

class TextEdit : public QPlainTextEdit
{
    Q_OBJECT

public:
    TextEdit();

private slots:
    void matchParentheses();

private:
    template<typename T>
    bool matchLeft(QTextBlock currentBlock, int i, int numLeftParentheses,char l, char r,T n);
     template<typename T>
    bool matchRight(QTextBlock currentBlock, int i, int numRightParentheses,char l, char r,T n);
    void createParenthesisSelection(int pos);
   /* bool matchLeftBrace(QTextBlock currentBlock, int index, int numRightBrace);
    bool matchRightBrace(QTextBlock currentBlock, int index, int numLeftBrace);
    void createBraceSelection(int pos);*/
};

#endif
