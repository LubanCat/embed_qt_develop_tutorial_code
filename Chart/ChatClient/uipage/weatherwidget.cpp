/******************************************************************
 Copyright (C) 2017 - All Rights Reserved by
 文 件 名 : weatherwidget.cpp --- WeatherWidget
 作 者    :
 编写日期 : 2017
 说 明    :
 历史纪录 :
 <作者>    <日期>        <版本>        <内容>

*******************************************************************/
#include "weatherwidget.h"
#include "unit.h"

WeatherWidget::WeatherWidget(QWidget *parent) :
    CustomDialog(parent)
{
    QNetworkAccessManager *manager= new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(replyFinished(QNetworkReply*)));
}

WeatherWidget::~WeatherWidget()
{

}

/**
 * @brief WeatherWidget::replyFinished
 * 数据解析
 * @param reply
 */
void WeatherWidget::replyFinished(QNetworkReply *reply)
{
    QJsonParseError jsonError;
    QJsonDocument jsonDocument=QJsonDocument::fromJson(reply->readAll(), &jsonError);
    if(jsonError.error==QJsonParseError::NoError)
    {
        if(jsonDocument.isObject())
        {
            // 天气数据
            QString currentCity = "";
            QString PM = "";
            QString date = "";
            QString weather = "";
            QString temperature = "";
            QString strWind     = "";

            QJsonObject jsonObj = jsonDocument.object();
            QJsonArray jsonResultArray =jsonObj["results"].toArray();

            for(int i = 0; i < jsonResultArray.count(); i++)
            {
                QJsonObject resultObj = jsonResultArray.at(i).toObject();
                currentCity = resultObj.value("currentCity").toString();
                PM = resultObj.value("pm25").toString();
                QJsonArray weather_dataArray = resultObj["weather_data"].toArray();
                QJsonObject weatherObj = weather_dataArray.at(0).toObject();
                date    = weatherObj.value("date").toString();
                weather = weatherObj.value("weather").toString();
                strWind = weatherObj.value("wind").toString();
                temperature = weatherObj.value("temperature").toString();
            }

            QString strWeatherInfo = "当前城市: ";
            strWeatherInfo.append(currentCity);
            strWeatherInfo.append(QString(", 实时空气质量: "));
            strWeatherInfo.append(PM);
            strWeatherInfo.append(QString(", 更新时间: "));
            strWeatherInfo.append(date);
            strWeatherInfo.append(QString(", 天气状况: "));
            strWeatherInfo.append(weather);
            strWeatherInfo.append(QString(", 风向: "));
            strWeatherInfo.append(strWind);
            strWeatherInfo.append(QString(", 温度: "));
            strWeatherInfo.append(temperature);
        }
    }
}
