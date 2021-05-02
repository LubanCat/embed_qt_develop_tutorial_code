#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "form.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void showchild();
    void hidechild();
    void sendText(const QString str);

private slots:
    void on_btn_open_1_clicked();
    void on_btn_close_1_clicked();
    void on_btn_send_1_clicked();

    void on_btn_open_2_clicked();
    void on_btn_close_2_clicked();
    void on_btn_send_2_clicked();

    void closeFrom_1();
    void closeFrom_2();

    void reiveText_1(const QString str);
    void reiveText_2(const QString str);


private:
    Ui::MainWindow *ui;

    Form *from_1=nullptr,*from_2=nullptr;
};

#endif // MAINWINDOW_H
