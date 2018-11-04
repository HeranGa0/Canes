#ifndef CANESFILE_H
#define CANESFILE_H
#include<QFileDialog>
#include<QTextEdit>
#include<QAction>
class CanesFile :public QObject
{
    Q_OBJECT
public:
    explicit CanesFile(QTextEdit* mw);
    void setSaveAsFileOn(bool on);
     void setOpenFileOn(bool on);
     void setSaveFileOn(bool on);
     void setNewFileOn(bool on);
private:
     QFile file{"/Users/herangao/build-FileTester-Desktop_Qt_5_9_1_clang_64bit-Debug/homa.txt"};
     void saveAsFile();
     void openFile();
     void saveFile();
     void newFile();
     QTextEdit* _mw;
     QAction* saveAs;
     QAction* fileOpen;
     QAction* save;
     QAction* news;
};

#endif // CANESFILE_H
