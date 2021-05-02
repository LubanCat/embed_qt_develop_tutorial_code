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

#include "customwidget.h"
#include "chatwindow.h"
#include "clientsocket.h"

#include <QButtonGroup>
#include <QSystemTrayIcon>

namespace Ui {
class MainWindow;
}

class MainWindow : public CustomMoveWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void SetSocket(ClientSocket *tcpSocket, const QString &name);
protected:
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *event);
    void showEvent(QShowEvent *event);
private:
    Ui::MainWindow *ui;

    // 按钮切换界面
    QButtonGroup *m_btnGroup;

    // 聊天窗管理
    QList<ChatWindow *> m_chatFriendWindows;
    QList<ChatWindow *> m_chatGroupWindows;

    // socket通信类
    ClientSocket    *m_tcpSocket;
    QSystemTrayIcon *systemTrayIcon;

    // 主动退出操作时不进行断线匹配
    bool            m_bQuit;
private slots:
    // 用户接受处理
    void SltReadMessages(const QJsonValue &json, const int &id);

    // 主界面切换
    void SltMainPageChanged(int index);

    // 好友列表点击
    void SltFriendsClicked(QQCell *cell);
    // 群组列表点击
    void SltGroupsClicked(QQCell *cell);

    // 右键菜单
    void onAddFriendMenuDidSelected(QAction *action);
    void onGroupPopMenuDidSelected(QAction *action);
    void onChildPopMenuDidSelected(QAction *action);

    // 消息窗口管理
    void SltFriendChatWindowClose();
    void SltGroupChatWindowClose();

    // 状态，上线下线状态
    void SltTcpStatus(const quint8 &state);
    // 解析Socket的消息
    void SltTcpReply(const quint8 &type, const QJsonValue &dataVal);

    // ---- 菜单管理信号槽 ----//
    void SltSysmenuCliecked(QAction *action);

    // 托盘菜单处理
    void SltTrayIcoClicked(QSystemTrayIcon::ActivationReason reason);
    void SltTrayIconMenuClicked(QAction *action);

    // 程序退出处理
    void SltQuitApp();

    // 头像裁剪ok
    void SltHeadPicCutOk();
    void SltUpdateUserHead(const int &userId, const QString &strHead);

private:
    // 添加系统菜单处理
    void InitSysMenu();
    void InitQQListMenu();
    void InitSysTrayIcon();

    // 以下便是每个消息类型的解析
    void ParseUpFriendHead(const QJsonValue &dataVal);

    void ParseAddFriendReply(const QJsonValue &dataVal);
    void ParseAddGroupReply(const QJsonValue &dataVal);
    void ParseCreateGroupReply(const QJsonValue &dataVal);

    // 好友添加请求确认
    void ParseAddFriendRequest(const QJsonValue &dataVal);
    void ParseAddGroupRequest(const QJsonValue &dataVal);

    // 获取
    void ParseGetFriendsReply(const QJsonValue &dataVal);
    void ParseGetGroupFriendsReply(const QJsonValue &dataVal);
    // 刷新
    void ParseRefreshFriendsReply(const QJsonValue &dataVal);
    void ParseRefreshGroupFriendsReply(const QJsonValue &dataVal);

    // 消息处理
    void ParseFriendMessageReply(const QJsonValue &dataVal);
    void ParseGroupMessageReply(const QJsonValue &dataVal);

    void AddMyGroups(const QJsonValue &dataVal);
    void UpdateFriendStatus(const quint8 &nStatus, const QJsonValue &dataVal);

    // 头像图片的获取
    QString GetHeadPixmap(const QString &name) const;
    void DownloadFriendHead(const int &userId, const QString &strHead);
};

#endif // MAINWINDOW_H
