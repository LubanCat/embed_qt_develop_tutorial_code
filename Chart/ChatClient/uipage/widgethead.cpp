/******************************************************************
 Copyright (C) 2017 - All Rights Reserved by
 文 件 名 : widgethead.cpp --- WidgetHead
 作 者    :
 编写日期 : 2017
 说 明    :
 历史纪录 :
 <作者>    <日期>        <版本>        <内容>

*******************************************************************/
#include "widgethead.h"
#include "myapp.h"
#include "unit.h"
#include "picturecutdialog.h"

#include <QEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QMouseEvent>

#include <QDebug>
#include <QCursor>
#include <QTimer>

#include "clientsocket.h"

WidgetHead::WidgetHead(QWidget *parent) :
    QWidget(parent)
{
    this->setFixedSize(66, 66);
    m_nType = Unknow;
    m_nUserId = MyApp::m_nId;

    pictureEdit = NULL;

    headUpLoadSocket = new ClientFileSocket(this);
    // 启动的时候就连接服务器，方便头像下载
    headUpLoadSocket->ConnectToServer(MyApp::m_strHostAddr, MyApp::m_nFilePort, -2);

    connect(headUpLoadSocket, SIGNAL(signalSendFinished()), this, SLOT(SltSendFileOk()));
    connect(headUpLoadSocket, SIGNAL(signalConnectd()), this, SLOT(SltConnectedToServer()));
    connect(headUpLoadSocket, SIGNAL(signamFileRecvOk(quint8,QString)), this, SLOT(SltFileRecvOk(quint8,QString)));

    dialog = new QDialog(this);
    dialog->resize(200, 200);
    dialog->hide();

    m_pixmap = QPixmap(":/resource/head/1.bmp");
    this->setCursor(Qt::PointingHandCursor);

    // 启动的时候对头像文件进行检测
    UnPackHeasicons();
}

/**
 * @brief WidgetHead::SetHeadPixmap
 * @param pixmap
 */
void WidgetHead::SetHeadPixmap(const QPixmap &pixmap)
{
    if (pixmap.isNull()) return;
    m_pixmap = pixmap;
    if (m_pixmap.width() > 64) {
        m_pixmap = m_pixmap.copy(0, 0, 64, 64) ;
    }

    update();
}

void WidgetHead::DownloadFriendHead(const int &userId)
{
    if (Unknow != m_nType) return;
    m_nType = GetFile;
    m_nUserId = userId;

    headUpLoadSocket->ConnectToServer(MyApp::m_strHostAddr, MyApp::m_nFilePort, -2);
}

/**
 * @brief WidgetHead::SltPictureCutClose
 * 关闭控制
 */
void WidgetHead::SltPictureCutClose()
{
    if (NULL != pictureEdit) {
        disconnect(pictureEdit, SIGNAL(signalClose()), this, SLOT(SltPictureCutClose()));
        disconnect(pictureEdit, SIGNAL(signalCutHeadOk()), this, SLOT(SltChangeNewPixHead()));
    }

    delete pictureEdit;
    pictureEdit = NULL;
}

/**
 * @brief WidgetHead::SltChangeNewPixHead
 * 有新的头像图片编辑成功
 */
void WidgetHead::SltChangeNewPixHead()
{
    this->SetHeadPixmap(MyApp::m_strHeadFile);
    m_nType = UpdateHeadPic;
    // 上传头像文件
    headUpLoadSocket->StartTransferFile(MyApp::m_strHeadFile);
}

void WidgetHead::SltBeginToSend()
{
    headUpLoadSocket->StartTransferFile(MyApp::m_strHeadFile);
}

/**
 * @brief WidgetHead::SltConnectedToServer
 *  连接上服务器了，可以上传头像了
 */
void WidgetHead::SltConnectedToServer()
{
    // 需要延迟1s左右上传，服务器更新较慢
    if (UpdateHeadPic == m_nType)
        QTimer::singleShot(1000, this, SLOT(SltBeginToSend()));
}

/**
 * @brief WidgetHead::SltSendFileOk
 * @param type
 * @param filePath
 */
void WidgetHead::SltSendFileOk()
{
    //    headUpLoadSocket->CloseConnection();
    m_nType = Unknow;
    Q_EMIT signalCutHeadOk();
}

/**
 * @brief WidgetHead::SltFileRecvOk
 * 文件接收完成
 */
void WidgetHead::SltFileRecvOk(const quint8 &, const QString &filePath)
{
    //    headUpLoadSocket->CloseConnection();
    m_nType = Unknow;
    if (MyApp::m_nId != m_nUserId) {
        // 头像上传完成，更新头像
        Q_EMIT signalUpdateUserHead(m_nUserId, filePath);
    }
    else {
        this->update();
    }
}

/**
 * @brief WidgetHead::UnPackHeasicons
 * 解压头像文件
 */
void WidgetHead::UnPackHeasicons()
{
    for (int i = 0; i < 10; i++) {
        QString strHead = MyApp::m_strHeadPath + QString("%1.bmp").arg(i);
        if (!QFile::exists(strHead)) {
            QPixmap pixmap(QString(":/resource/head/%1.bmp").arg(i));
            pixmap.save(MyApp::m_strHeadPath + QString("%1.bmp").arg(i));
        }
    }
}

/**
 * @brief WidgetHead::enterEvent
 * @param e
 */
void WidgetHead::enterEvent(QEvent *e)
{
    Q_UNUSED(e)
    //    dialog->show();
    //    dialog->move(MyApp::m_nWinX - dialog->width() - 30, MyApp::m_nWinY + 50);
}

/**
 * @brief WidgetHead::leaveEvent
 * @param e
 */
void WidgetHead::leaveEvent(QEvent *e)
{
    //    dialog->hide();
    Q_UNUSED(e)
}

/**
 * @brief WidgetHead::mousePressEvent
 * @param e
 */
void WidgetHead::mousePressEvent(QMouseEvent *e)
{
    if (Qt::LeftButton == e->button()) {
        if (NULL == pictureEdit) {
            pictureEdit = new PictureCutDialog();
            connect(pictureEdit, SIGNAL(signalClose()), this, SLOT(SltPictureCutClose()));
            connect(pictureEdit, SIGNAL(signalCutHeadOk()), this, SLOT(SltChangeNewPixHead()));
        }

        pictureEdit->show();
    }
}

/**
 * @brief WidgetHead::paintEvent
 */
void WidgetHead::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(QColor("#FF0000"));
    painter.fillRect(this->rect(), QColor("#F1F2F3"));

    painter.drawPixmap(1, 1, 64, 64, m_pixmap);
}
