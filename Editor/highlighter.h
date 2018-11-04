#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>
#include <QVector>

class QString;
class QTextDocument;

struct ParenthesisInfo
{
    char character;
    int position;
};

struct BraceInfo
{
    char character;
    int position;
};
/*struct ParenthesisInfo
{
    char character;
    int position;
};*/
class TextBlockData : public QTextBlockUserData
{
public:
    TextBlockData();

    template<typename T>
    auto various (T n) {
        return nullptr;
    }


    auto various (ParenthesisInfo n) {
        return &m_parentheses;
    }


     auto various(BraceInfo n) {
        return &m_Brace;
    }

    //QVector<BraceInfo*> brace();
    template<typename T>
     void insert(T *info, QVector<T*> * vec)
    {
        int i = 0;
        while (i < vec->size() &&
            info->position > vec->at(i)->position)
            ++i;

        vec->insert(i, info);
    }

private:
    QVector<ParenthesisInfo *> m_parentheses;
     QVector<BraceInfo *> m_Brace;
};

class QTextDocument;

class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    Highlighter(QTextDocument *parent = 0);

protected:
    void highlightBlock(const QString &text) override;

private:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QRegularExpression commentStartExpression;
    QRegularExpression commentEndExpression;

    QTextCharFormat keywordFormat;
    QTextCharFormat classFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;
    QTextCharFormat macroFormat;
};

#endif // HIGHLIGHTER_H
