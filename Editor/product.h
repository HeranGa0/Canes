#ifndef PRODUCT_H
#define PRODUCT_H
#include<QObject>
#include<QTextEdit>
class Product :public QObject
{
    Q_OBJECT
public:
    explicit Product(QTextEdit* mw);

private:

     QTextEdit* _mw;
     QAction* run;
     //QAction* fileOpen;
};

#endif // PRODUCT_H
