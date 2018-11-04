#include"product.h"
#include"mainwindow.h"
#include<QDebug>
#include<QMenuBar>

Product::Product(QTextEdit *mw):QObject(mw),_mw(mw) {
    auto mWindow = dynamic_cast<MainWindow*>(_mw->parent());
    auto mbar = mWindow->menuBar();
    if(!mbar) {
        qDebug() << "__sFILE";
    }
    auto fileMenu = mbar->addMenu("Product");
   run = fileMenu->addAction("Run");

    connect(run,&QAction::triggered,mWindow,&MainWindow::evaluation);

}

