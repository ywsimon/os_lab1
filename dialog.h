#ifndef DIALOG_H
#define DIALOG_H
#include <QDialog>
#include<Windows.h>

namespace Ui {
class Dialog;
}


extern HANDLE empty_Semaphore;//设置信号量用。empty_Semaphore表示空的缓冲池的数量
extern HANDLE full_Semaphore;//用full_Semaphore表示满的缓冲池的数量
extern HANDLE mutex_Semaphore;//用mutex_Semaphore表示互斥信号量
extern QString logmessage;

class Dialog : public QDialog

{

    Q_OBJECT

public:

    explicit  Dialog(QWidget *parent = nullptr);
     ~Dialog();

    void start(int x =0);
    static  void  Producer(Dialog*);
    static void Consumer(Dialog*);


private slots:
    void  setptag();
    void  setctag();
    void  setbar();


signals:
     void setprocess();
private:
     Ui::Dialog *ui;

     HANDLE handle[2];//创建HANDLE数组handle
     int maxbufferlen=0;

     int sum=0;
     int del=0;
     int cur=0;
     bool ptag=true;
     bool ctag=true;
};
#endif // DIALOG1_H
