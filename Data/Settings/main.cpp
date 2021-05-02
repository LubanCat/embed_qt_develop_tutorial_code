/******************************************************************
 Copyright (C) 2019 - All Rights Reserved by
 文 件 名 : main.cpp ---
 作 者    : Niyh(lynnhua)
 论 坛    : http://www.firebbs.cn
 编写日期 : 2019
 说 明    :
 历史纪录 :
 <作者>    <日期>        <版本>        <内容>
  Niyh	   2019    	1.0.0 1     文件创建
*******************************************************************/
#include "settingwidget.h"
#include "skin.h"

#include "appconfig.h"

#include <QTextCodec>
#include <QApplication>
#include <QDateTime>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setFont(QFont(Skin::m_strAppFontNormal));

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF8"));
    AppConfig::m_strSettingsFile = a.applicationDirPath() + "/conf/config.ini";
    //app 启动次数
    AppConfig::SaveSetting("Etc", "runtimes",AppConfig::ReadSetting("Etc", "runtimes").toInt()+1);

    QString msg = QString("当前时间: %1  %2").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")).arg("启动程序");
    qDebug() << msg;

    SettingWidget w;

#ifdef __arm__
    w.showFullScreen();
#else
    w.resize(800, 480);
    w.show();
#endif

    return a.exec();
}
