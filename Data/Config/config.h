/***************************************************************************
 * Copyright (C) 2021 - All Rights Reserved by
 * 文 件 名 : config.h --- Config
 * 作 者    : taotaodiy(野火电子)
 * 论 坛    : http://www.firebbs.cn
 * 文 档    : https://doc.embedfire.com/linux/qt/embed/zh/latest/index.html
 * 编写日期 : 2021/04/10
 * 说 明    :
 * 历史纪录 :
 * <作者>        <版本>        <日期>                 <内容>
 * taotaodiy     1.0.0 1      2021/04/10            文件创建
 ****************************************************************************/
#ifndef CONFIG_H
#define CONFIG_H

#include <QVariant>
#include <QMutexLocker>
#include <QSharedPointer>

class Config
{
public:
    static QSharedPointer<Config>& instance()
    {
        if (self.isNull())
        {
            QMutexLocker mutexLocker(&m_Mutex);
            if (self.isNull())
                self = QSharedPointer<Config>(new Config());
        }
        return self;
    }

private:
    Config();
    Config(const Config&){}
    Config& operator==(const Config&){}
private:
    static QMutex m_Mutex;
    static QSharedPointer<Config> self;

public:
    static QString m_strSettingsFile;
    void SaveSetting(const QString &group, const QString &name, const QVariant &value);
    QVariant ReadSetting(const QString &group, const QString &name, const QVariant &value = QVariant());

#if defined(Q_OS_WIN32) //注册表
    void SaveRegistry(const QString &position, const QString &name, const QVariant &value);
    QVariant ReadRegistry(const QString &position, const QString &name, const QVariant &value = QVariant());
#endif

    static int m_iRunTimes;
    static int m_iRunTime;
    static bool m_bAutoLogon;
    static bool m_bRememberPasswd;
    static bool m_bMouseInsert;
    static bool m_bPlayVideo;

    static QSize GetDesktopSize();
};

#endif // CONFIG_H
