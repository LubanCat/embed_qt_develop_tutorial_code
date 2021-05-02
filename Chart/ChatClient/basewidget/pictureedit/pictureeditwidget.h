/******************************************************************
 Copyright (C) 2017 - All Rights Reserved by
 文 件 名 : pictureeditwidget.h --- PictureEditWidget
 作 者    :
 编写日期 : 2017
 说 明    :
 历史纪录 :
 <作者>    <日期>        <版本>        <内容>

*******************************************************************/
#ifndef PICTUREEDITWIDGET_H
#define PICTUREEDITWIDGET_H

#include <QWidget>

class CutDialog;
class PhotoShotDialog;

namespace Ui {
class PictureEditWidget;
}

class PictureEditWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PictureEditWidget(QWidget *parent = 0);
    ~PictureEditWidget();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::PictureEditWidget *ui;

    QPixmap scaledPix;
    PhotoShotDialog *pdialog;

private:
    void LoadPicture();
private slots:
     void SltCutDlgMoved(QRect rect);
     void on_btnLoad_clicked();
     void on_btnSave_clicked();
};

#endif // PICTUREEDITWIDGET_H
