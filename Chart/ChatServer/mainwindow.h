/******************************************************************
 Copyright (C) 2017 - All Rights Reserved by
 文 件 名 : mainwindow.h --- MainWindow
 作 者    :
 编写日期 : 2017
 说 明    :
 历史纪录 :
 <作者>    <日期>        <版本>        <内容>

*******************************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include "customwidget.h"

#include <QButtonGroup>
#include <QSystemTrayIcon>
#include <QStandardItemModel>

class TcpMsgServer;
class TcpFileServer;

namespace Ui {
class MainWindow;
}

class MainWindow : public CustomMoveWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);

private slots:
    void SltChangePages(int index);

    void on_btnBackup_clicked();

    void on_btnDataUdo_clicked();

    void on_btnLogin_clicked();
    void on_btnExit_clicked();
    void on_btnQuit_clicked();

    // 系统菜单
    void SltTrayIcoClicked(QSystemTrayIcon::ActivationReason reason);
    void SltTrayIconMenuClicked(QAction *action);
    void ShowUserStatus(const QString &text);
    void SltTableClicked(const QModelIndex &index);
    void on_btnUserRefresh_clicked();

    void on_btnUserInsert_clicked();

private:
    Ui::MainWindow *ui;

    QButtonGroup *m_buttonGroup;
    QStandardItemModel *m_model;

    // 服务配置
    TcpMsgServer *tcpMsgServer;
    TcpFileServer *tcpFileServer;

    // 系统菜单
    QSystemTrayIcon *systemTrayIcon;
private:
    void InitNetwork();
    void SetUserIdentity(const int &identity);
protected:
    int m_nTimerId;
    void closeEvent(QCloseEvent *e);
    void timerEvent(QTimerEvent *e);
};

#endif // MAINWINDOW_H
