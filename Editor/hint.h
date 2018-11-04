#ifndef HINT_H
#define HINT_H
#include<string>
#include <QMainWindow>
#include<QListWidget>
#include<QTextEdit>
class Hint :public QObject
{
    Q_OBJECT
public:
    explicit Hint(QTextEdit* mw);
    ~Hint();
private:
    QTextEdit* _mw;
    QListWidget *listview;
    void reMatch(QString &text);
    QListWidget *listWidget;
    QString lastpiece ="";
    //bool eventFilter(QObject *obj, QEvent *event);
    QString tobeCompleted;
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif // HINT_H
