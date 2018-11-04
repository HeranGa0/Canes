#include"canesfile.h"
#include<QMenu>
#include <sstream>
#include <QTextStream>
#include<QMenuBar>
#include<QDebug>
#include"mainwindow.h"
CanesFile::CanesFile(QTextEdit* mw ):QObject(mw->parent()), _mw(mw) {

    qDebug()<< _mw->parent()->objectName();
    auto mbar = (dynamic_cast<QMainWindow*>(_mw->parent()))->menuBar();
    auto fileMenu = mbar->addMenu("File");
   saveAs = fileMenu->addAction("save as");
   fileOpen = fileMenu->addAction("open");
   news = fileMenu->addAction("new");
}

void CanesFile::setSaveAsFileOn(bool on) {
    if(on) {
        connect(saveAs,&QAction::triggered,this,&CanesFile::saveAsFile);

    }
    else {
        disconnect(saveAs,&QAction::triggered,this,&CanesFile::saveAsFile);
    }
}
void CanesFile::saveAsFile() {
    auto text = _mw->toPlainText();
    if (!file.open(QIODevice::WriteOnly)) {



       } else {

            std::string line;
             QTextStream out(&file);
             auto tex = text.toStdString();
             std::istringstream iss(tex);
            while( std::getline(iss,line)) {


                  out<<line.data()<<"\n";
           }
       }file.close();
QString fileName = QFileDialog::getSaveFileName(dynamic_cast<QWidget*>( _mw->parent()), tr("SaveAs File"),
                           "",
                           tr("saveAs"));

file.rename(fileName);
}

void CanesFile::saveFile() {

}

void CanesFile::setSaveFileOn(bool on) {
    if(on) {
        connect(save,&QAction::triggered,this,&CanesFile::saveFile);

    }
    else {
         disconnect(save,&QAction::triggered,this,&CanesFile::saveFile);
    }
}

void CanesFile::setNewFileOn(bool on) {
    if(on) {
        connect(news,&QAction::triggered,this,&CanesFile::newFile);

    }
    else {
         disconnect(news,&QAction::triggered,this,&CanesFile::newFile);
    }
}

void CanesFile::setOpenFileOn(bool on) {
    if(on) {
        connect(fileOpen,&QAction::triggered,this,&CanesFile::openFile);

    }
    else {
         disconnect(fileOpen,&QAction::triggered,this,&CanesFile::openFile);
    }
}

void CanesFile::openFile() {
    auto filename = QFileDialog::getOpenFileName();
    QFile of(filename);
    if (!of.open(QIODevice::ReadOnly)) {



       }

        QTextStream in(&of);



              _mw->insertPlainText(in.readAll());

           of.close();
}

void CanesFile::newFile() {
    auto fileName = QFileDialog::getSaveFileName(NULL, "Create New File","","");
    qDebug()<<fileName;
    auto mwindow = dynamic_cast<MainWindow*>(_mw->parent());
    if(!mwindow) {
        qDebug()<<"convertion mainWindow failed";
    }
    mwindow->createNewEditor(fileName);
}
