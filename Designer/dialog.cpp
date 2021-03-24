#include "dialog.h"
#include "ui_dialog.h"

#include <QMessageBox>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
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
