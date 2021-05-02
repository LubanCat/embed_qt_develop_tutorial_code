/******************************************************************
 Copyright (C) 2017 - All Rights Reserved by
 文 件 名 : systemsetting.h --- SystemSetting
 作 者    :
 编写日期 : 2017
 说 明    :
 历史纪录 :
 <作者>    <日期>        <版本>        <内容>

*******************************************************************/
#ifndef SYSTEMSETTING_H
#define SYSTEMSETTING_H

#include "customwidget.h"

namespace Ui {
class SystemSetting;
}

class SystemSetting : public CustomDialog
{
    Q_OBJECT

public:
    explicit SystemSetting(QWidget *parent = 0);
    ~SystemSetting();
signals:
    void signalRotate();
protected:
    void changeEvent(QEvent *e);

private slots:
    void on_btnSaveSetting_clicked();

    void on_toolButton_clicked();

    void on_toolBtnLeftClr_clicked();

    void on_toolBtnClr_clicked();

private:
    Ui::SystemSetting *ui;
};

#endif // SYSTEMSETTING_H
