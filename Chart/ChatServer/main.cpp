/******************************************************************
 Copyright (C) 2017 - All Rights Reserved by
 文 件 名 : main.cpp ---
 作 者    :
 编写日期 : 2017
 说 明    :
    利用数据库MySQL实现系统基本功能：
    教务人员管理模块（包括教务人员、教师、员工的信息查询，员工成绩查询等操作）和
    教师管理模块（员工成绩的录入、添加、修改、删除、查询，教师个人信息的查询和修改等）
 历史纪录 :
 <作者>    <日期>        <版本>        <内容>

*******************************************************************/
#include "mainwindow.h"
#include "myapp.h"
#include "databasemagr.h"

#include <QApplication>
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>

#include <QImage>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyApp::InitApp(a.applicationDirPath());

    QFile qss(":/qss/resource/qss/default.css");
    qss.open(QIODevice::ReadOnly);
    a.setStyleSheet(qss.readAll());
    qss.close();

    // 加载数据库
    DataBaseMagr::Instance()->OpenDb(MyApp::m_strDatabasePath + "info.db");

    MainWindow w;
    w.show();

    return a.exec();
}
