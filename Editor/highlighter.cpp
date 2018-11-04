#include "highlighter.h"

TextBlockData::TextBlockData()
{
    // Nothing to do
}

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{

    HighlightingRule rule;

    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;
    keywordPatterns << "\\bclass\\b"
                    << "\\bfunction"
                    << "\\bextends\\b"
                    <<"\\bnew\\b"
                    <<"\\bwhile\\b"
                      <<"\\bfor\\b"
                     <<"#insert"
                     ;
    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegularExpression("\\bQ[A-Za-z]+\\b");
    rule.format = classFormat;
    highlightingRules.append(rule);



    multiLineCommentFormat.setForeground(Qt::red);

    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegularExpression("\".*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::blue);
    rule.pattern = QRegularExpression("\\b[A-Za-z0-9_]+(?=\\()");
    rule.format = functionFormat;
    highlightingRules.append(rule);

    /*macroFormat.setForeground(Qt::red);
    rule.pattern = QRegularExpression("<.*>");
    rule.format = macroFormat;
    highlightingRules.append(rule);*/

    singleLineCommentFormat.setForeground(Qt::green);
    rule.pattern = QRegularExpression("//[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);



    commentStartExpression = QRegularExpression("/\\*");
    commentEndExpression = QRegularExpression("\\*/");
}

void Highlighter::highlightBlock(const QString &text)
{
    TextBlockData *data = new TextBlockData;
      auto process = [this,data,text](char charactorL,char charactorR,    auto* vec,auto infor) {
        int leftPos = text.indexOf(charactorL);
        typedef decltype(infor)  T;
        while (leftPos != -1) {
            T *info = new T;
            info->character = charactorL;
            info->position = leftPos;

            data->insert(info,vec);
            leftPos = text.indexOf(charactorL, leftPos + 1);
        }

        int rightPos = text.indexOf(charactorR);
        while (rightPos != -1) {
            T *info = new T;
            info->character = charactorR;
            info->position = rightPos;

            data->insert(info,vec);

            rightPos = text.indexOf(charactorR, rightPos +1);
        }
        };
      process('(',')',data->various(ParenthesisInfo()),ParenthesisInfo());
      process('{','}',data->various(BraceInfo()),BraceInfo());
        setCurrentBlockUserData(data);
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(commentStartExpression);

    while (startIndex >= 0) {
        QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                            + match.capturedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);


    }
}
