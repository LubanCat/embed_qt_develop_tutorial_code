/******************************************************************
 Copyright (C) 2017 - All Rights Reserved by
 文 件 名 : chatwindow.h --- ChatWindow
 作 者    :
 编写日期 : 2017
 说 明    :
 历史纪录 :
 <作者>    <日期>        <版本>        <内容>

*******************************************************************/
#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include "customwidget.h"
#include "clientsocket.h"
#include "qqcell.h"

#include <QStandardItemModel>
#include <QTime>

namespace Ui {
class ChatWindow;
}

//////////////////////////////////////////////////////////////////////////////////////////
/// \brief The ChatWindow class
/// 聊天界面窗口类
class ChatWindow : public CustomMoveWidget
{
    Q_OBJECT

public:
    explicit ChatWindow(QWidget *parent = 0);
    ~ChatWindow();

    void SetCell(QQCell *cell, const quint8 &type = 0);
    QString GetIpAddr() const;
    int GetUserId() const;

    // 接受服务器转发过来的信息
    void AddMessage(const QJsonValue &json);
    // 更新群组人员的状态
    void UpdateUserStatus(const QJsonValue &dataVal);
signals:
    void signalClose();
    // 发送给服务器的消息
    void signalSendMessage(const quint8 &type, const QJsonValue &json);
protected:
    void changeEvent(QEvent *e);
    void keyPressEvent(QKeyEvent *event);
private:
    Ui::ChatWindow *ui;

    // 用户相关信息保存
    QQCell *m_cell;
    QStandardItemModel *m_model;

    // 文件传输
    ClientFileSocket *m_tcpFileSocket;
    QString          m_strFileName;
    QTime            m_updateTime;

    quint8          m_nFileType;

    quint8          m_nChatType;        // 聊天类型，群组聊天或私人聊天

private slots:
    void SltChatMessage(const QString &text);
    void on_btnSendMsg_clicked();
    void SltCloseWindow();
    void on_toolButton_7_clicked();
    // 文件
    void SltFileRecvFinished(const quint8 &type, const QString &filePath);
    void SltUpdateProgress(quint64 bytes, quint64 total);

    void on_toolButton_6_clicked();

    void on_toolButton_clicked();

    void on_btnSendFile_clicked();

    void SltDownloadFiles(const QString &fileName);

    void on_toolButton_4_clicked();

    void on_toolButton_3_clicked();

public slots:

private:
    QString GetHeadPixmap(const QString &name) const;
};

#endif // CHATWINDOW_H
