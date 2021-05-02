#include "mainwindow.h"
#include "myevent.h"
#include "ui_mainwindow.h"

#include <QMouseEvent>
#include <QMouseEvent>

#include <QDebug>
#include <QDateTime>
#include <QMessageBox>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->installEventFilter(this);

    timer = new QTimer();
    connect(timer,SIGNAL(timeout()), this, SLOT(handletimeout()));
    timer->start(5000);
}

MainWindow::~MainWindow()
{
    delete ui;
}
bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
    if (object == this && event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_Space) {
            QString mesg=QString("空格按键");
            ui->statusBar->showMessage(mesg);
            ui->textEdit->append(QString("[%1] %2").arg(QDateTime::currentDateTime().toString("hh:mm:ss")).arg(mesg));
            return true;
        } else
            return false;
    }
    return false;
}

void MainWindow::mousePressEvent(QMouseEvent * event)
{
    QString mesg;

    if(event->button() == Qt::LeftButton)
        mesg=QString("鼠标左键:(%1,%2)").arg(event->x()).arg(event->y());
    else if(event->button() == Qt::RightButton)
        mesg=QString("鼠标右键:(%1,%2)").arg(event->x()).arg(event->y());
    else if(event->button() == Qt::MiddleButton)
        mesg=QString("鼠标滚轮:(%1,%2)").arg(event->x()).arg(event->y());

    ui->statusBar->showMessage(mesg);
    ui->textEdit->append(QString("[%1] %2").arg(QDateTime::currentDateTime().toString("hh:mm:ss")).arg(mesg));
}
void MainWindow::mouseReleaseEvent(QMouseEvent * event)
{
    qDebug() << "Release" << event->x() << ":" << event->y();
}
void MainWindow::mouseMoveEvent(QMouseEvent * event)
{
    QString mesg=QString("鼠标移动:(%1,%2)").arg(event->x()).arg(event->y());

    ui->statusBar->showMessage(mesg);
    ui->textEdit->append(QString("[%1] %2").arg(QDateTime::currentDateTime().toString("hh:mm:ss")).arg(mesg));
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    QString mesg=QString("鼠标双击:(%1,%2)").arg(event->x()).arg(event->y());

    ui->statusBar->showMessage(mesg);
    ui->textEdit->append(QString("[%1] %2").arg(QDateTime::currentDateTime().toString("hh:mm:ss")).arg(mesg));
}
void MainWindow::wheelEvent(QWheelEvent *event)
{
    QString mesg;

    if(event->delta()>0)
        mesg=QString("滚轮上滚");
    else
        mesg=QString("滚轮下滚");

    ui->statusBar->showMessage(mesg);
    ui->textEdit->append(QString("[%1] %2").arg(QDateTime::currentDateTime().toString("hh:mm:ss")).arg(mesg));
}

void MainWindow::keyPressEvent(QKeyEvent * event)
{
    QString mesg=QString("键盘按下:(keyvalue,%1)").arg(event->key());

    ui->statusBar->showMessage(mesg);
    ui->textEdit->append(QString("[%1] %2").arg(QDateTime::currentDateTime().toString("hh:mm:ss")).arg(mesg));

}

bool MainWindow::event(QEvent *event)
{
    QString mesg=QString("事件类型:(eventype,%1)").arg(event->type());
    qDebug()<<mesg;

    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *ke = static_cast<QKeyEvent *>(event);
        if (ke->key() == Qt::Key_Tab) {
            QString mesg=QString("tab按键:(eventype,%1)").arg(event->type());
            ui->statusBar->showMessage(mesg);
            ui->textEdit->append(QString("[%1] %2").arg(QDateTime::currentDateTime().toString("hh:mm:ss")).arg(mesg));
            return true;
        }
    }
    if (event->type() == MyEvent::myEventype())
    {
        MyEvent *myEvent = dynamic_cast<MyEvent*>(event);
        QString mesg=QString("自定义事件");
        ui->statusBar->showMessage(mesg);
        ui->textEdit->append(QString("[%1] %2").arg(QDateTime::currentDateTime().toString("hh:mm:ss")).arg(mesg));
        return true;
    }

    return QWidget::event(event);
}
void MainWindow::resizeEvent(QResizeEvent *event)
{
    QString mesg=QString("窗口大小:(%1,%2)").arg(this->width()).arg(this->height());

    ui->statusBar->showMessage(mesg);
    ui->textEdit->append(QString("[%1] %2").arg(QDateTime::currentDateTime().toString("hh:mm:ss")).arg(mesg));
}
void MainWindow::paintEvent(QPaintEvent *event)
{
//    QString mesg=QString("绘图事件");

//    ui->statusBar->showMessage(mesg);
//    ui->textEdit->append(QString("[%1] %2").arg(QDateTime::currentDateTime().toString("hh:mm:ss")).arg(mesg));

//    event->accept();
}
void MainWindow::closeEvent(QCloseEvent *event)
{
    QString dlgTitle="question";
    QString str="关闭窗口";

    QString mesg=QString("关闭窗口事件");
    ui->statusBar->showMessage(mesg);
    ui->textEdit->append(QString("[%1] %2").arg(QDateTime::currentDateTime().toString("hh:mm:ss")).arg(mesg));

    int choose=QMessageBox::question(this, dlgTitle, str,QMessageBox::Yes,QMessageBox::No);

    switch (choose) {
    case QMessageBox::Yes:
        event->accept();
        break;
    case QMessageBox::No:
        event->ignore();
        break;
    default:
        event->accept();
        break;
    }
}
void MainWindow::handletimeout()
{
    MyEvent *mypostEvent = new MyEvent();
    //postevent
    QCoreApplication::postEvent(this, mypostEvent);

    //sendEvent
    MyEvent *sendsendEvent = new MyEvent();
    qDebug() <<QCoreApplication::sendEvent(this, sendsendEvent);
    delete sendsendEvent;
}
