/******************************************************************
 Copyright (C) 2017 - All Rights Reserved by
 文 件 名 : pictureeditwidget.cpp --- PictureEditWidget
 作 者    :
 编写日期 : 2017
 说 明    :
 历史纪录 :
 <作者>    <日期>        <版本>        <内容>

*******************************************************************/
#include "pictureeditwidget.h"
#include "ui_pictureeditwidget.h"
#include "photoshotdialog.h"

#include <QFileDialog>
#include <QPixmap>

#include <QMessageBox>

#include <QDebug>

PictureEditWidget::PictureEditWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PictureEditWidget)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);

    //该类为剪切框
    pdialog = new PhotoShotDialog(ui->labelPreview);
    connect(pdialog, SIGNAL(signalMoved(QRect)), this, SLOT(SltCutDlgMoved(QRect)));
}

PictureEditWidget::~PictureEditWidget()
{
    delete ui;
}

void PictureEditWidget::changeEvent(QEvent *e)
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

void PictureEditWidget::LoadPicture()
{

}

/**
 * @brief PictureEditWidget::SltCutDlgMoved
 * 移动的时候裁剪图片
 * @param rect
 */
void PictureEditWidget::SltCutDlgMoved(QRect rect)
{
    QPixmap pix = scaledPix.copy(rect);
    //剪切出的图片的保存地址
    ui->label64->setPixmap(pix.scaled(64, 64));
    ui->label48->setPixmap(pix.scaled(48, 48));
}

/**
 * @brief PictureEditWidget::on_btnLoad_clicked
 * 加载本地图片
 */
void PictureEditWidget::on_btnLoad_clicked()
{
    // 加载图片
    static QString s_strPath = "./";
    QString strFileName = QFileDialog::getOpenFileName(this,
                                                       tr("选择图片"),
                                                       s_strPath,
                                                       tr("图片(*.jpg;*.png)"));
    // 如果没选择文件直接退出
    if (strFileName.isEmpty()) {
        return;
    }

    s_strPath = strFileName;
    // 这里是原图的地址
    QPixmap pixmap(strFileName);
    // 非空判断
    if (!pixmap.isNull())
    {
        int nW = ui->widgetLoad->width();
        int nH = ui->widgetLoad->height();
        if ((pixmap.width() > nW) && (pixmap.height() > nH))
        {
            ui->labelPreview->resize(QSize(nW, nH));
            scaledPix = pixmap.scaled(nW, nH, Qt::KeepAspectRatio);
        }
        else {
            ui->labelPreview->resize(pixmap.size());
            scaledPix = pixmap;
        }

        ui->labelPreview->setPixmap(scaledPix);
    }
    else {

    }

    pdialog->show();
}

/**
 * @brief PictureEditWidget::on_btnSave_clicked
 * 保存裁剪的头像，并上传服务器,
 */
void PictureEditWidget::on_btnSave_clicked()
{
    QPixmap pixmap = scaledPix.copy(pdialog->getShotGeometry()).scaled(64, 64);
    if (!pixmap.isNull()) {
        pixmap.save("./head-64.png", "png");
    }

    pixmap = scaledPix.copy(pdialog->getShotGeometry()).scaled(48, 48);
    if (!pixmap.isNull()) {
        pixmap.save("./head-48.png", "png");
    }
}
