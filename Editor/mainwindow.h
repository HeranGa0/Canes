#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<string>
#include <QMainWindow>
#include "highlighter.h"
#include<QPair>
#include"hint.h"
#include"canesfile.h"
#include"product.h"
#include"codeeditor.h"
#include"canesmenubase.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void evaluation();
    void createNewEditor(const QString& fileName);
   std::string passage;
   EditorInterface ei;
   CanesMenus* cFile;
   public slots:
      void testNewEd (QListWidgetItem* i);
private:
   CodeEditor* ce;
    Ui::MainWindow *ui;


    void insertMenu();


     QFile file{"/Users/herangao/build-FileTester-Desktop_Qt_5_9_1_clang_64bit-Debug/homa.txt"};
      CanesFile* fm = nullptr;
      Product* pt = nullptr;
    void setFileMenuOn(bool i);
    void setFileOpenOn(bool i);
    void setFileNewsOn(bool i);


protected:
    bool eventFilter(QObject *obj, QEvent *event);

private:
    void findNext();
    void replaceAll();
    void replace();
    void findPrevious();

};


#endif // MAINWINDOW_H
