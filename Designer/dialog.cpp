#include "dialog.h"
#include "ui_dialog.h"

#include <QMessageBox>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    QPixmap map(":/img/passwd.png");
    map.scaled(40,40);
    ui->label_passwd->setPixmap(map);

    ui->pushButton_login->setStyleSheet(QString("QPushButton{border-image: url(:/images/keyled/ic_btn.png);}"
                                         "QPushButton:pressed{border-image: url(:/images/keyled/ic_btn_pre.png);}"));
    ui->pushButton_logout->setStyleSheet(QString("QPushButton{border-image: url(:/images/keyled/ic_btn.png);}"
                                         "QPushButton:pressed{border-image: url(:/images/keyled/ic_btn_pre.png);}"));

}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_login_clicked()
{
    if(ui->lineEdit_account->text().isEmpty())
    {
        QMessageBox::information(this,"提示","账号不能为空",QMessageBox::Yes);
        return ;
    }
    if(ui->lineEdit_passwd->text().isEmpty())
    {
        QMessageBox::information(this,"提示","密码不能为空",QMessageBox::Yes);
        return;
    }

    accept();
}
