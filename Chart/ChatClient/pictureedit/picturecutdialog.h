/******************************************************************
 Copyright (C) 2017 - All Rights Reserved by
 文 件 名 : picturecutdialog.h --- PictureCutDialog
 作 者    :
 编写日期 : 2017
 说 明    :
 历史纪录 :
 <作者>    <日期>        <版本>        <内容>

*******************************************************************/
#ifndef PICTURECUTDIALOG_H
#define PICTURECUTDIALOG_H

#include "customwidget.h"

class CutDialog;
class PhotoShotDialog;

namespace Ui {
class PictureCutDialog;
}

class PictureCutDialog : public CustomMoveWidget
{
    Q_OBJECT

public:
    explicit PictureCutDialog(QWidget *parent = 0);
    ~PictureCutDialog();

signals:
    void signalCutHeadOk();
    void signalClose();

protected:
    void changeEvent(QEvent *e);

private slots:
    void SltCutDlgMoved(QRect rect);

    void on_btnLoad_clicked();

    void on_btnSave_clicked();

private:
    Ui::PictureCutDialog *ui;

    QPixmap scaledPix;
    PhotoShotDialog *pdialog;
};

#endif // PICTURECUTDIALOG_H
