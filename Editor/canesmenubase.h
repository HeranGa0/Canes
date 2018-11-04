#ifndef CANESMENUBASE_H
#define CANESMENUBASE_H
#include<QFileDialog>
#include<QMainWindow>
#include<QAction>
#include<QMenu>
#include<QLineEdit>
class CanesMenuBase:public QObject
{
    Q_OBJECT
public:
    CanesMenuBase(QMainWindow* mWindow=nullptr,QString tag = QString("None"));
    void insertMenuOption(QString name, std::function<void()> lamb);
 private:
    QMainWindow* _mWindow;
    QMenu* menu;
};

class EditFindReplace:public QObject
{
    Q_OBJECT
public:
    //EditFindReplace(Q)
private:
    QLineEdit* _find;
    QLineEdit* _replace;
};

class CanesMenus
{
public:
    CanesMenus(QMainWindow* mWindow=nullptr);
};

#endif // CANESMENUBASE_H
