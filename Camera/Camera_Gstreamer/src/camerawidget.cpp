/******************************************************************
 Copyright (C) 2017 - All Rights Reserved by
 文 件 名 : camerawidget.cpp ---
 作 者    : Niyh  (QQ:393320854)
 编写日期 : 2019
 说 明    :
 历史纪录 :
 <作者>    <日期>        <版本>        <内容>
           2019/9/15
           2021/2/22
*******************************************************************/
#include "camerawidget.h"
#include "skin.h"

#include <QDateTime>
#include <QPainter>
#include <QPaintEvent>
#include <QDebug>
#include <QMouseEvent>
#include <QApplication>
#include <QCameraInfo>
#include <QHBoxLayout>

#include <QCoreApplication>

#include "qtswitchbutton.h"


CameraWidget::CameraWidget(QWidget *parent) : QtAnimationWidget(parent)
{
    m_cmd = new QProcess(this);

    QTimer::singleShot(500, this, SLOT(on_timeout()));
}

CameraWidget::~CameraWidget()
{
    if  (m_cmd->state() != QProcess::NotRunning) {
        m_cmd->kill();
        qDebug() << "kill";
    }
}

void CameraWidget::InitCamera()
{
    //m_cmd->start("gst-launch-1.0 -v v4l2src device=/dev/video0 ! fbdevsink");
    m_cmd->start("gst-launch-1.0 -v v4l2src device=/dev/video1 ! \"video/x-raw, format=(string)YUY2, width=(int)1280, height=(int)720, framerate=(fraction)15/1\" ! videoconvert ! fbdevsink");

}

void CameraWidget::on_timeout()
{
    InitCamera();
}

void CameraWidget::showEvent(QShowEvent *e)
{
    if  (m_cmd->state() == QProcess::NotRunning) {
        //m_cmd->start("gst-launch-1.0 -v v4l2src device=/dev/video0 ! fbdevsink");
        m_cmd->start("gst-launch-1.0 -v v4l2src device=/dev/video1 ! \"video/x-raw, format=(string)YUY2, width=(int)1280, height=(int)720, framerate=(fraction)15/1\" ! videoconvert ! fbdevsink");
    }

    QWidget::showEvent(e);
}

void CameraWidget::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);

}

void CameraWidget::mousePressEvent(QMouseEvent *e)
{
    if  (m_cmd->state() != QProcess::NotRunning) {
        m_cmd->kill();
    }
    emit signalBackHome();
}

void CameraWidget::mouseReleaseEvent(QMouseEvent *e)
{
    QWidget::mouseReleaseEvent(e);
}

void CameraWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.scale(m_scaleX, m_scaleY);
    painter.fillRect(QRect(0, 0, m_nBaseWidth, m_nBaseHeight), QColor("#000000"));

    //绘制按钮
    drawToolButton(&painter);
}

void CameraWidget::drawToolButton(QPainter *painter)
{
}

