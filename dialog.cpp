#include "ui_dialog.h"
#include "dialog.h"
#include<QDebug>
#include<QThread>
#include<QString>
#include<QTextBrowser>
#include<QProgressBar>
 HANDLE empty_Semaphore;//设置信号量用。empty_Semaphore表示空的缓冲池的数量
 HANDLE full_Semaphore;//用full_Semaphore表示满的缓冲池的数量
 HANDLE mutex_Semaphore;//用mutex_Semaphore表示互斥信号量
 QString logmessage;


Dialog::Dialog(QWidget *parent)
    : QDialog(parent),

    ui(new Ui::Dialog)

{
    ui->setupUi(this);
    setWindowTitle("producer&consumer");
    connect(this->ui->pushButton,SIGNAL(clicked()),this,SLOT(setptag()));
    connect(this->ui->pushButton_2,SIGNAL(clicked()),this,SLOT(setctag()));
    connect(this->ui->label_7,SIGNAL(windowIconTextChange()),this,SLOT(setprocess()));
    connect(this,SIGNAL(setprocess()),this,SLOT(setbar()));
}

Dialog::~Dialog()

{
    delete ui;
}

void Dialog:: setbar(){
    this->ui->progressBar->setValue(((double)cur/(double)maxbufferlen)*100);
}
void Dialog:: start(int x){
    this->ui->label_10->setNum(x);
    maxbufferlen=x;

    empty_Semaphore = CreateSemaphore(nullptr, x, x, nullptr);//创建信号量empty_Semaphore
    full_Semaphore = CreateSemaphore(nullptr, 0, x, nullptr);//创建信号量full_Semaphore
    mutex_Semaphore = CreateSemaphore(nullptr, 1, 1, nullptr);//创建互斥信号量mutex_Semaphore

    //创建生产者和消费者线程
    CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)Producer, this, 0, nullptr);

    handle[1] =CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)(Consumer), this, 0, nullptr);

    WaitForMultipleObjects(2, handle, TRUE, -1);//主进程等待线程的结束
}

 void Dialog::Producer(Dialog* ptr){
     int i=-1;
    while(true){

        if(ptr->ptag==false){
            QThread::msleep(500);
            continue;
        }
        i++;

        WaitForSingleObject(empty_Semaphore, -1);//对empty_Semaphore进行P操作
        WaitForSingleObject(mutex_Semaphore, -1);//对mutex_Semaphore进行P操作

      QString x;
      x="produce  ";
      QString y = QString::number(1+(i%ptr->maxbufferlen), 10);
      QString z="product  ing...";
      logmessage=x+y+z;
      ptr->ui->textBrowser->append("<font color=\"#00FF00\">" +logmessage);
      ptr->sum++;
      ptr->cur++;
      ptr->ui->label_5->setNum(ptr->sum);
      QString yy = QString::number(ptr->cur, 10);
      ptr->ui->label_7->setText(yy);
       emit ptr->setprocess();
      //ptr->setprocess();
      //ptr->ui->progressBar->setValue(((double)ptr->cur/(double)ptr->maxbufferlen)*100);
      ReleaseSemaphore(mutex_Semaphore, 1, nullptr);//对mutex_Semaphore进行V操作
      ReleaseSemaphore(full_Semaphore, 1, nullptr);//对full_Semaphore进行V操作
      QThread::msleep(500);
    }
}

 void Dialog::Consumer(Dialog*ptr){
    int j=0;
    while(true){
        if(ptr->ctag==false){
            QThread::msleep(500);
            continue;
        }

        j++;
        WaitForSingleObject(full_Semaphore, -1);   //对full_Semaphore进行P操作
        WaitForSingleObject(mutex_Semaphore, -1);//对mutex_Semaphore进行P操作

        QString x="consume  ";
        QString y = QString::number(1+(j%ptr->maxbufferlen), 10);
        QString z="product  ing";
        logmessage=x+y+z;
        ptr->ui->textBrowser->append("<font color=\"#FF0000\">" +logmessage);
        ptr->del++;
        ptr->cur--;
        ptr->ui->label_6->setNum(ptr->del);
        QString yy = QString::number(ptr->cur, 10);
        ptr->ui->label_7->setText(yy);
        emit ptr->setprocess();
        //ptr->setprocess();
        //ptr->ui->progressBar->setValue((int((double)ptr->cur/(double)ptr->maxbufferlen)*100));
        ReleaseSemaphore(mutex_Semaphore, 1, nullptr);//对mutex_Semaphore进行V操作
        ReleaseSemaphore(empty_Semaphore, 1, nullptr);//对empty_Semaphore进行V操作
        QThread::msleep(500);
    }
}


 void Dialog :: setptag(){
      if(ptag==true){
          ptag=false;
          this->ui->pushButton->setText("continue produce line");
      }
      else{
          ptag=true;
          this->ui->pushButton->setText("hang up produce line");
      }
  }

  void Dialog:: setctag(){
      if(ctag==true){
          ctag=false;
          this->ui->pushButton_2->setText("continue consume line");
      }
      else{
          ctag=true;
          this->ui->pushButton_2->setText("hang up consume line");
      }
  }
