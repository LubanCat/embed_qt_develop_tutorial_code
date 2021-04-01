#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    from_1 = new Form();
    from_2 = new Form();

    from_1->hide();
    from_2->hide();
    from_1->setSendStr("子窗口1呼叫主窗口","子窗口1呼叫子窗口2");
    from_2->setSendStr("子窗口2呼叫主窗口","子窗口2呼叫子窗口1");
    ui->btn_send_1->setEnabled(false);
    ui->btn_send_2->setEnabled(false);

    connect(this,SIGNAL(showchild()),from_2,SLOT(showMyself()));
    connect(this,SIGNAL(hidechild()),from_2,SLOT(hideMyself()));
    connect(this,SIGNAL(sendText(const QString)),from_2,SLOT(reiveLabText(const QString)));

    connect(from_1,SIGNAL(close()),this,SLOT(closeFrom_1()));
    connect(from_2,SIGNAL(close()),this,SLOT(closeFrom_2()));
    connect(from_1,SIGNAL(sendMainText(const QString)),this,SLOT(reiveText_1(const QString)));
    connect(from_2,SIGNAL(sendMainText(const QString)),this,SLOT(reiveText_2(const QString)));
    connect(from_1,SIGNAL(sendChildText(const QString)),from_2,SLOT(reiveChildText(const QString)));
    connect(from_2,SIGNAL(sendChildText(const QString)),from_1,SLOT(reiveChildText(const QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_open_1_clicked()
{
    from_1->show();
    ui->btn_send_1->setEnabled(true);
}

void MainWindow::on_btn_close_1_clicked()
{
    from_1->hide();
    ui->btn_send_1->setEnabled(false);
}

void MainWindow::on_btn_send_1_clicked()
{
    from_1->setLabText("主窗口呼叫窗口1");
}

void MainWindow::on_btn_open_2_clicked()
{
    emit showchild();
    ui->btn_send_2->setEnabled(true);
}

void MainWindow::on_btn_close_2_clicked()
{
    emit hidechild();
    ui->btn_send_2->setEnabled(false);
}

void MainWindow::on_btn_send_2_clicked()
{
    emit sendText("主窗口呼叫窗口2");
}

void MainWindow::closeFrom_1()
{
    ui->btn_send_1->setEnabled(false);
}
void MainWindow::closeFrom_2()
{
    ui->btn_send_2->setEnabled(false);
}
void MainWindow::reiveText_1(const QString str)
{
    ui->lab_1->setText(str);
}
void MainWindow::reiveText_2(const QString str)
{
    ui->lab_2->setText(str);
}
