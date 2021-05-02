/******************************************************************
 Copyright (C) 2017 - All Rights Reserved by
 文 件 名 : systemsetting.cpp --- SystemSetting
 作 者    :
 编写日期 : 2017
 说 明    :
 历史纪录 :
 <作者>    <日期>        <版本>        <内容>

*******************************************************************/
#include "systemsetting.h"
#include "ui_systemsetting.h"

#include "myapp.h"
#include "global.h"

#include <QMessageBox>
#include <QPropertyAnimation>
#include <QColorDialog>

SystemSetting::SystemSetting(QWidget *parent) :
    CustomDialog(parent),
    ui(new Ui::SystemSetting)
{
    ui->setupUi(this);

    // 加载之前的配置
    ui->lineEditHostAddr->setText(MyApp::m_strHostAddr);
    ui->lineEditHostMsgPort->setText(QString::number(MyApp::m_nMsgPort));
    ui->lineEditHostFilePort->setText(QString::number(MyApp::m_nFilePort));
}

SystemSetting::~SystemSetting()
{
    delete ui;
}

void SystemSetting::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

// 保存ip地址配置
void SystemSetting::on_btnSaveSetting_clicked()
{
    QString strHost = ui->lineEditHostAddr->text();

    // 判断是否ip地址
    if (!myHelper::IsIP(strHost))
    {
        CMessageBox::Infomation(this, tr("IP地址设置有误!"));
        return;
    }

    int nMsgPort = ui->lineEditHostMsgPort->text().toInt();
    if (nMsgPort > 65535 || nMsgPort < 100) {
        CMessageBox::Infomation(this, tr("端口设置有误!"));
        return;
    }

    int nFilePort = ui->lineEditHostFilePort->text().toInt();
    if (nFilePort > 65535 || nFilePort < 100) {
        CMessageBox::Infomation(this, tr("端口设置有误!"));
        return;
    }

    if (nMsgPort == nFilePort) {
        CMessageBox::Infomation(this, tr("2个端口不能设置一样!"));
        return;
    }

    MyApp::m_strHostAddr = strHost;
    MyApp::m_nMsgPort    = nMsgPort;
    MyApp::m_nFilePort   = nFilePort;

    // 保存配置
    MyApp::SaveConfig();

    CMessageBox::Infomation(this, tr("配置保存成功，重启生效!"));
}

/**
 * @brief SystemSetting::on_toolButton_clicked
 * 颜色配置
 */
void SystemSetting::on_toolButton_clicked()
{
    QString strClr = ui->lineEditClrBackground->text();

    QColor color = QColorDialog::getColor(QColor(strClr), this, tr("背景色配置"));
    if (!color.isValid()) {
        return;
    }

    ui->lineEditClrBackground->setText(color.name().toUpper());
    ui->labelBackground->setStyleSheet(QString("background-color: %1").arg(color.name()));
}

/**
 * @brief SystemSetting::on_toolBtnLeftClr_clicked
 */
void SystemSetting::on_toolBtnLeftClr_clicked()
{
    QString strClr = ui->lineEditLeftBubble->text();

    QColor color = QColorDialog::getColor(QColor(strClr), this, tr("气泡颜色配置"));
    if (!color.isValid()) {
        return;
    }

    ui->lineEditLeftBubble->setText(color.name().toUpper());
    ui->labelLeftBubbleClr->setStyleSheet(QString("background-color: %1").arg(color.name()));
}

/**
 * @brief SystemSetting::on_toolBtnClr_clicked
 */
void SystemSetting::on_toolBtnClr_clicked()
{
    QString strClr = ui->lineEditRightBubble->text();

    QColor color = QColorDialog::getColor(QColor(strClr), this, tr("气泡颜色配置"));
    if (!color.isValid()) {
        return;
    }

    ui->lineEditRightBubble->setText(color.name().toUpper());
    ui->labelRightBubbleClr->setStyleSheet(QString("background-color: %1").arg(color.name()));
}
