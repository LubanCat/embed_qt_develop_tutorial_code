#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMouseEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowFlags (Qt::FramelessWindowHint);

    m_move = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

//拖拽操作
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        windowsDrag = true;
        //获得鼠标的初始位置
        mouseStartPoint = event->globalPos();
        //mouseStartPoint = event->pos();
        //获得窗口的初始位置
        windowTopLeftPoint = this->frameGeometry().topLeft();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(windowsDrag)
    {
        //获得鼠标移动的距离
        QPoint distance = event->globalPos() - mouseStartPoint;
        //QPoint distance = event->pos() - mouseStartPoint;
        //改变窗口的位置
        this->move(windowTopLeftPoint + distance);
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        windowsDrag = false;
    }
}

void MainWindow::on_tbtn_main_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_tbtn_kill_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_tbtn_clean_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_tbtn_repari_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_tbtn_accelerate_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}

void MainWindow::on_tbtn_function_clicked()
{

}

void MainWindow::on_tbtn_vault_clicked()
{

}

void MainWindow::on_tbtn_soft_clicked()
{

}

void MainWindow::on_tbtn_game_clicked()
{

}

void MainWindow::on_toolButton_27_clicked()
{
    exit(0);
}
