#include<canesmenubase.h>
#include"mainwindow.h"
#include<QMenuBar>
#include<QDebug>
#include <sstream>
#include<QString>
#include<QMessageBox>
CanesMenuBase::CanesMenuBase(QMainWindow *mWindow,QString tag ) {
     _mWindow = mWindow;
     menu =_mWindow->menuBar()->addMenu(tag);
 }

void CanesMenuBase::insertMenuOption(QString name, std::function<void ()> lamb) {
    auto afmw = dynamic_cast<MainWindow*>(_mWindow);
    if(!afmw) {
        qDebug()<<"CanesMenubase.cpp";
    }
    connect(menu->addAction(name),&QAction::triggered,lamb);
}

CanesMenus::CanesMenus(QMainWindow *mWindow) {
    auto afmw = dynamic_cast<MainWindow*>(mWindow);
    if(!afmw) {
        qDebug()<<"CanesMenubase.cpp";
    }
    auto cFile = new CanesMenuBase(mWindow,"File");
    cFile->insertMenuOption("new",[afmw]() {
        auto fileName = QFileDialog::getSaveFileName(NULL, "Create New File","","");
        QFile temp(fileName);
        temp.open(QIODevice::WriteOnly);
        qDebug()<<fileName;
        afmw->createNewEditor(fileName);
        temp.close();
    });
    cFile->insertMenuOption("open",[afmw]() {
        auto filename = QFileDialog::getOpenFileName();
        QFile of(filename);
        if (!of.open(QIODevice::ReadOnly)) {



           }

            QTextStream in(&of);
           (afmw->createNewEditor(filename));
           afmw->ei.displayedCE()->insertPlainText(in.readAll());
            of.close();

    });

    cFile->insertMenuOption("save as",[afmw]() {
        auto displayedCe = afmw->ei.displayedCE();
        if(!displayedCe) {
            QMessageBox m{QMessageBox::NoIcon,"No File is selected!","No File is selected!"};
            m.exec();
            return;
        }
        auto text = displayedCe->toPlainText();
        if (!displayedCe->file.open(QIODevice::WriteOnly)) {
           } else {

                std::string line;
                 QTextStream out(&(displayedCe->file));
                 auto tex = text.toStdString();
                 std::istringstream iss(tex);
                while( std::getline(iss,line)) {


                      out<<line.data()<<"\n";
               }
           }displayedCe->file.close();
    QString fileName = QFileDialog::getSaveFileName(dynamic_cast<QWidget*>( afmw->parent()), QObject::tr("SaveAs File"),
                               "",
                               QObject::tr("saveAs"));

    displayedCe->file.rename(fileName);
    });

    cFile->insertMenuOption("save",[afmw]() {
        auto displayedCe = afmw->ei.displayedCE();
        if(!displayedCe) {
            QMessageBox m{QMessageBox::NoIcon,"No File is selected!","No File is selected!"};
            m.exec();
            return;
        }
        auto text = displayedCe->toPlainText();
        if (!displayedCe->file.open(QIODevice::WriteOnly)) {
           } else {
                displayedCe->file.resize(0);
                std::string line;
                 QTextStream out(&(displayedCe->file));
                 qDebug()<<displayedCe->file.fileName();
                 auto tex = text.toStdString();
                 std::istringstream iss(tex);
                while( std::getline(iss,line)) {


                      out<<line.data()<<"\n";
               }
           }displayedCe->file.close();
    });

    auto cEdit = new CanesMenuBase(mWindow,"Edit");
    cEdit->insertMenuOption("Find",[afmw]() {

    });

     auto cProduct = new CanesMenuBase(mWindow,"Product");
     cProduct->insertMenuOption("Run",[afmw]() {
         auto displayedCe = afmw->ei.displayedCE();
         if(!displayedCe) {
             QMessageBox m{QMessageBox::NoIcon,"No File is selected!","No File is selected!"};
             m.exec();
             return;
         }
         afmw->evaluation();
     });
}
