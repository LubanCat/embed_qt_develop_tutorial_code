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

#ifdef __arm__
#define DEFAULT_DEVICE      1
#else
#define DEFAULT_DEVICE      0
#endif

CameraWidget::CameraWidget(QWidget *parent) : QtAnimationWidget(parent)
{
    surface = NULL;

    QTimer::singleShot(500, this, SLOT(on_timeout()));
}

CameraWidget::~CameraWidget()
{
    delete surface;
}

void CameraWidget::InitCamera(const QCameraInfo &cameraInfo)
{
    m_camera.reset(new QCamera(cameraInfo));
    connect(m_camera.data(), QOverload<QCamera::Error>::of(&QCamera::error), this, &CameraWidget::displayCameraError);

    if (NULL != m_camera) {
        //surface = new QtVideoWidgetSurface(this);
        //surface = new QCameraViewfinder(this);

        m_imageCapture.reset(new QCameraImageCapture(m_camera.data()));

        connect(m_imageCapture.data(), &QCameraImageCapture::readyForCaptureChanged, this, &CameraWidget::readyForCapture);
        //connect(m_imageCapture.data(), &QCameraImageCapture::imageCaptured, this, &CameraWidget::processCapturedImage);
        connect(m_imageCapture.data(), &QCameraImageCapture::imageSaved, this, &CameraWidget::imageSaved);
        connect(m_imageCapture.data(), QOverload<int, QCameraImageCapture::Error, const QString &>::of(&QCameraImageCapture::error),this, &CameraWidget::displayCaptureError);


        surface = new QtViewFinder(this);
        connect(surface,SIGNAL(returnbtn_clicked_signal()),this, SLOT(on_clieckBackhome()));
        connect(surface,SIGNAL(take_picture_signal()),this, SLOT(on_takePicture()));
        surface->setGeometry(0,0,this->width(),this->height());
        surface->setEnabledTake(false);

        m_camera->setViewfinder(surface);

        QCameraViewfinderSettings set;
        set.setResolution(1280, 720);
        m_camera->setViewfinderSettings(set);

        surface->show();
        qDebug()<<surface->width()<<surface->height();

//        QCameraViewfinderSettings set = m_camera->viewfinderSettings();
//        qDebug()<<set.resolution().height()<<set.resolution().width();
        surface->setSize(surface->width(),surface->height());

        m_camera->start();
    }
}

void CameraWidget::on_timeout()
{
    const QList<QCameraInfo> availableCameras = QCameraInfo::availableCameras();
    for (const QCameraInfo &cameraInfo : availableCameras) {
        if (cameraInfo == QCameraInfo::defaultCamera())
            qDebug()<<"defaultCamera ok";
    }
    InitCamera(QCameraInfo::defaultCamera());
}

void CameraWidget::on_clieckBackhome()
{
    m_camera->stop();
    emit signalBackHome();
}
void CameraWidget::on_takePicture()
{
    //获取程序当前运行目录
    QString strFilePath = QCoreApplication::applicationDirPath();
    strFilePath+="/photos/";
    QString strFileName;
    strFileName += QDateTime::currentDateTime().toString("yyyyMMddhhmss");
    strFileName += ".png";
    m_imageCapture->capture(strFilePath+strFileName);


//    QCameraFocus *focus = m_camera->focus();
//    focus->setFocusPointMode(QCameraFocus::FocusPointCustom);
//    focus->setCustomFocusPoint(QPointF(0.25f, 0.75f)); // A point near the bottom left, 25% away from the corner, near that shiny vase
}
void CameraWidget::readyForCapture(bool ready)
{
    qDebug()<<"ready:"<<ready;
    surface->setEnabledTake(ready);
}
void CameraWidget::imageSaved(int id, const QString &fileName)
{
    qDebug()<<"id:"<<id<<"fileName:"<<fileName;

    Q_UNUSED(id);
//    ui->statusbar->showMessage(tr("Captured \"%1\"").arg(QDir::toNativeSeparators(fileName)));

//    m_isCapturingImage = false;
//    if (m_applicationExiting)
//        close();
    surface->loadImage(fileName);
}
void CameraWidget::processCapturedImage(int requestId, const QImage& img)
{
    qDebug()<<"requestId:"<<requestId;

    Q_UNUSED(requestId);
    QImage scaledImage = img.scaled(surface->size(),
                                    Qt::KeepAspectRatio,
                                    Qt::SmoothTransformation);

    //ui->lastImagePreviewLabel->setPixmap(QPixmap::fromImage(scaledImage));

    //Display captured image for 4 seconds.
    //displayCapturedImage();
    //QTimer::singleShot(4000, this, &Camera::displayViewfinder);
}
void CameraWidget::displayCaptureError(int id, const QCameraImageCapture::Error error, const QString &errorString)
{
    qDebug()<<"id:"<<id<<"error:"<<error<<"errorString:"<<errorString;

    Q_UNUSED(id);
    Q_UNUSED(error);
    //QMessageBox::warning(this, tr("Image Capture Error"), errorString);
    //m_isCapturingImage = false;
}
void CameraWidget::displayCameraError()
{
    qDebug()<< m_camera->errorString();

    m_camera->stop();
    //emit signalBackHome();
    //this->~CameraWidget();
}


//void CameraWidget::SltClearMessage()
//{
//    m_strInfoMsg.clear();
//    this->update();
//}

void CameraWidget::showEvent(QShowEvent *e)
{
    QWidget::showEvent(e);
}

void CameraWidget::resizeEvent(QResizeEvent *e)
{
    SetScaleValue();
    QWidget::resizeEvent(e);

}

void CameraWidget::mousePressEvent(QMouseEvent *e)
{
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
}

void CameraWidget::drawToolButton(QPainter *painter)
{
}

