#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QDebug>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(this->ui->pushButton,SIGNAL(clicked()),this,SLOT(starttest()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete  test;
}

void MainWindow :: starttest(){
    QString t=this->ui->lineEdit->text();
    int x=t.toInt();
    test=new Dialog();
    test->show();
    test->start(x);

}
