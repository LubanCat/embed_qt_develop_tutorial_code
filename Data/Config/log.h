/***************************************************************************
 * Copyright (C) 2021 - All Rights Reserved by
 * 文 件 名 : logwidget.h ---
 * 作 者    : taotaodiy(野火电子)
 * 论 坛    : http://www.firebbs.cn
 * 文 档    : https://doc.embedfire.com/linux/qt/embed/zh/latest/index.html
 * 编写日期 : 2021/04/10
 * 说 明    :
 * 历史纪录 :
 * <作者>        <版本>        <日期>                 <内容>
 * taotaodiy     1.0.0 1      2021/04/10            文件创建
 ****************************************************************************/
#ifndef LOGWIDGET_H
#define LOGWIDGET_H

#include <QObject>
#include <QTextEdit>
#include <QMutexLocker>
#include <QSharedPointer>
#include <QString>
#include <QFile>

class Log : public QObject
{
    Q_OBJECT
public:
    static QSharedPointer<Log>& instance()
    {
        if (self.isNull())
        {
            QMutexLocker mutexLocker(&m_Mutex);
            if (self.isNull())
                self = QSharedPointer<Log>(new Log());
        }
        return self;
    }
private:
    Log(QObject *parent = nullptr);
    Log(const Log&){}
    Log& operator==(const Log&){}
public:
    void InstallLog();
    void UninstallLog();
    void SaveLog(QString log);
    void SetAllowEmit(bool state);
signals:
    void sendLog(const QString &log);

public:
    QString logstr;
private:
    bool allow;

    QString logfile;
    QString logpath;
    QString logname;

    QFile *file;

    static QMutex m_Mutex;
    static QSharedPointer<Log> self;
};

#endif // LOGWIDGET_H
