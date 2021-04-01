#include "form.h"
#include "ui_form.h"

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
}

Form::~Form()
{
    delete ui;
}

void Form::closeEvent(QCloseEvent*event)
{
    emit close();
}

void Form::setLabText(const QString str)
{
    ui->label->setText(str);
}
void Form::setSendStr(QString str_1, QString str_2)
{
    sendstr_1=str_1;
    sendstr_2=str_2;
}

void Form::hideMyself()
{
    this->hide();
}

void Form::showMyself()
{
    this->show();
}
void Form::reiveLabText(const QString str)
{
    ui->label->setText(str);
}
void Form::reiveMainText(const QString str)
{
    ui->label->setText(str);
}

void Form::reiveChildText(const QString str)
{
    ui->label->setText(str);
}
void Form::on_btn_hide_clicked()
{
    this->close();
}
void Form::on_btn_send_1_clicked()
{
    emit sendMainText(sendstr_1);
}

void Form::on_btn_send_2_clicked()
{
    emit sendChildText(sendstr_2);
}
