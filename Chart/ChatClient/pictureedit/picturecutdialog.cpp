/******************************************************************
 Copyright (C) 2017 - All Rights Reserved by
 文 件 名 : picturecutdialog.cpp --- PictureCutDialog
 作 者    :
 编写日期 : 2017
 说 明    :
 历史纪录 :
 <作者>    <日期>        <版本>        <内容>

*******************************************************************/
#include "picturecutdialog.h"
#include "ui_picturecutdialog.h"
#include "photoshotdialog.h"
#include "myapp.h"

#include <QFileDialog>


PictureCutDialog::PictureCutDialog(QWidget *parent) :
    CustomMoveWidget(parent),
    ui(new Ui::PictureCutDialog)
{
    ui->setupUi(this);

    this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);
    connect(ui->btnWinClose, SIGNAL(clicked(bool)), this, SIGNAL(signalClose()));
    pdialog = NULL;
}

PictureCutDialog::~PictureCutDialog()
{
    delete ui;
}

void PictureCutDialog::changeEvent(QEvent *e)
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

/**
 * @brief PictureCutDialog::on_btnLoad_clicked
 */
void PictureCutDialog::on_btnLoad_clicked()
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

    if (NULL == pdialog) {
        //该类为剪切框
        pdialog = new PhotoShotDialog(ui->labelPreview);
        connect(pdialog, SIGNAL(signalMoved(QRect)), this, SLOT(SltCutDlgMoved(QRect)));
        pdialog->show();
    }
}

/**
 * @brief PictureCutDialog::SltCutDlgMoved
 * @param rect
 */
void PictureCutDialog::SltCutDlgMoved(QRect rect)
{
    QPixmap pix = scaledPix.copy(rect);
    //剪切出的图片的保存地址
    ui->label64->setPixmap(pix.scaled(64, 64));
    ui->label48->setPixmap(pix.scaled(48, 48));
}

/**
 * @brief PictureCutDialog::on_btnSave_clicked
 * 保存图片
 */
void PictureCutDialog::on_btnSave_clicked()
{
    if (NULL == pdialog) return;

    QPixmap pixmap = scaledPix.copy(pdialog->getShotGeometry()).scaled(64, 64);
    if (!pixmap.isNull()) {
        pixmap.save(MyApp::m_strHeadPath + QString("%1_head_64.png").arg(MyApp::m_nId), "png");
        MyApp::m_strHeadFile = MyApp::m_strHeadPath + QString("%1_head_64.png").arg(MyApp::m_nId);
    }

    // 聊天窗口需求的图片
    pixmap = scaledPix.copy(pdialog->getShotGeometry()).scaled(48, 48);
    if (!pixmap.isNull()) {
        pixmap.save(MyApp::m_strHeadPath + QString("%1_head_48.png").arg(MyApp::m_nId), "png");
    }

    Q_EMIT signalCutHeadOk();
}
