#include <QtGui>

#include "textedit.h"
#include "highlighter.h"

TextEdit::TextEdit()
{
    connect(this, SIGNAL(cursorPositionChanged()),
            this, SLOT(matchParentheses()));
}

// walk through and check that we don't exceed 80 chars per line
void TextEdit::matchParentheses()
{
    bool match = false;
    QList<QTextEdit::ExtraSelection> selections;
    setExtraSelections(selections);

    TextBlockData *data = static_cast<TextBlockData *>(textCursor().block().userData());

    if (data) {
        auto procedure = [this,data](auto ty,char l, char r) {
            typedef decltype(ty) T;
        QVector<T *> infos = *data->various(T());
        //QVector<BraceInfo*> bInfos = data->brace();
        int pos = textCursor().block().position();

        for (int i = 0; i < infos.size(); ++i) {
            T *info = infos.at(i);

            int curPos = textCursor().position() - textCursor().block().position();
            if (info->position == curPos - 1 && info->character == l) {
                if (matchLeft(textCursor().block(), i + 1, 0,l,r,T()))
                    createParenthesisSelection(pos + info->position);
            } else if (info->position == curPos - 1 && info->character == r) {
                if (matchRight(textCursor().block(), i - 1, 0,l,r,T()))
                    createParenthesisSelection(pos + info->position);
            }
        }
        };
        procedure(ParenthesisInfo(),'(',')');
        procedure(BraceInfo(),'{','}');
    }
}
template<typename T>
bool TextEdit::matchLeft(QTextBlock currentBlock, int i, int numLeftParentheses,char l, char r,T n)
{
    TextBlockData *data = static_cast<TextBlockData *>(currentBlock.userData());
    QVector<T *> infos = *data->various(T());

    int docPos = currentBlock.position();
    for (; i < infos.size(); ++i) {
        T *info = infos.at(i);

        if (info->character == l) {
            ++numLeftParentheses;
            continue;
        }

        if (info->character == r && numLeftParentheses == 0) {
            createParenthesisSelection(docPos + info->position);
            return true;
        } else
            --numLeftParentheses;
    }

    currentBlock = currentBlock.next();
    if (currentBlock.isValid())
        return matchLeft(currentBlock, 0, numLeftParentheses,l,r,T());

    return false;
}

template<typename T>
bool TextEdit:: matchRight(QTextBlock currentBlock, int i, int numRightParentheses,char l, char r,T n)
{
    TextBlockData *data = static_cast<TextBlockData *>(currentBlock.userData());
    QVector<T *> parentheses = *data->various(T());

    int docPos = currentBlock.position();
    for (; i > -1 && parentheses.size() > 0; --i) {
        T *info = parentheses.at(i);
        if (info->character == r) {
            ++numRightParentheses;
            continue;
        }
        if (info->character == l && numRightParentheses == 0) {
            createParenthesisSelection(docPos + info->position);
            return true;
        } else
            --numRightParentheses;
    }

    currentBlock = currentBlock.previous();
    if (currentBlock.isValid())
        return matchRight(currentBlock, 0, numRightParentheses,l,r,T());

    return false;
}

void TextEdit::createParenthesisSelection(int pos)
{
    QList<QTextEdit::ExtraSelection> selections = extraSelections();

    QTextEdit::ExtraSelection selection;
    QTextCharFormat format = selection.format;
    format.setBackground(Qt::green);
    selection.format = format;

    QTextCursor cursor = textCursor();
    cursor.setPosition(pos);
    cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
    selection.cursor = cursor;

    selections.append(selection);

    setExtraSelections(selections);
}
