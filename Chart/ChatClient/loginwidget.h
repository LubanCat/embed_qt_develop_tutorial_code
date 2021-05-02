/******************************************************************
 Copyright (C) 2017 - All Rights Reserved by
 文 件 名 : loginwidget.h --- LoginWidget
 作 者    :
 编写日期 : 2017
 说 明    :
 历史纪录 :
 <作者>    <日期>        <版本>        <内容>

*******************************************************************/
#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include "customwidget.h"
#include "clientsocket.h"
#include "mainwindow.h"

namespace Ui {
class LoginWidget;
}

/////////////////////////////////////////////////////////////////////////
/// \brief The LoginWidget class
/// 登陆界面设计
class LoginWidget : public CustomMoveWidget
{
    Q_OBJECT

public:
    explicit LoginWidget(QWidget *parent = 0);
    ~LoginWidget();

signals:
    void signalRotate();
protected:
    void changeEvent(QEvent *e);

private slots:
    // 登陆
    void on_btnLogin_clicked();
    // 注册
//    void on_btnRegedit_clicked();
    // 服务器信息返回处理
    void SltTcpStatus(const quint8 &state);
    void on_btnCancel_clicked();

    void on_btnSaveCfg_clicked();

    void on_btnWinMenu_clicked();

    void SltAnimationFinished();

    void SltEditFinished();
    void on_checkBoxPasswd_clicked(bool checked);

    void on_checkBoxAutoLogin_clicked(bool checked);

private:
    Ui::LoginWidget *ui;

    bool m_bConnected;
    // socket通信
    ClientSocket *m_tcpSocket;

    void InitWidget();
protected:
    void paintEvent(QPaintEvent *);
};

#endif // LOGINWIDGET_H
