/******************************************************************
 Copyright (C) 2017 - All Rights Reserved by
 文 件 名 : weatherwidget.h --- WeatherWidget
 作 者    :
 编写日期 : 2017
 说 明    :
 历史纪录 :
 <作者>    <日期>        <版本>        <内容>

*******************************************************************/
#ifndef WEATHERWIDGET_H
#define WEATHERWIDGET_H

#include "customwidget.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>

class QLabel;
class QPushButton;

class WeatherWidget : public CustomDialog
{
    Q_OBJECT
public:
    explicit WeatherWidget(QWidget *parent = 0);
    ~WeatherWidget();

signals:

private slots:
    void replyFinished(QNetworkReply* reply);

private:
    QLabel *labelTemperature;
    QLabel *labelCity;


    QNetworkAccessManager *manager;
};

#endif // WEATHERWIDGET_H
