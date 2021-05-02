/***************************************************************************
 * Copyright (C) 2021 - All Rights Reserved by
 * 文 件 名 : config.cpp --- Config
 * 作 者    : taotaodiy(野火电子)
 * 论 坛    : http://www.firebbs.cn
 * 文 档    : https://doc.embedfire.com/linux/qt/embed/zh/latest/index.html
 * 编写日期 : 2021/04/10
 * 说 明    :
 * 历史纪录 :
 * <作者>        <版本>        <日期>                 <内容>
 * taotaodiy     1.0.0 1      2021/04/10            文件创建
 ****************************************************************************/
#include "config.h"
#include <QSettings>
#include <QApplication>

#include <QtDebug>

#ifdef __arm__
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <linux/fb.h>
#endif

QMutex Config::m_Mutex;
QSharedPointer<Config> Config::self;

QString Config::m_strSettingsFile = "conf/config.ini";

int     Config::m_iRunTimes=0;
int     Config::m_iRunTime=0;
bool    Config::m_bAutoLogon = false;
bool    Config::m_bRememberPasswd = false;
bool    Config::m_bMouseInsert = false;
bool    Config::m_bPlayVideo = false;

Config::Config()
{

}

void Config::SaveSetting(const QString &group, const QString &name, const QVariant &value)
{
    QSettings settings(m_strSettingsFile, QSettings::IniFormat);
    settings.beginGroup(group);
    settings.setValue(name, value);
    settings.endGroup();
}

QVariant Config::ReadSetting(const QString &group, const QString &name, const QVariant &value)
{
    QSettings settings(m_strSettingsFile, QSettings::IniFormat);
    settings.beginGroup(group);
    QVariant keyValue = settings.value(name, value);
    settings.endGroup();

    return keyValue;
}

#if defined(Q_OS_WIN32)
void Config::SaveRegistry(const QString &position, const QString &name, const QVariant &value)
{
    QSettings settings(position, QSettings::NativeFormat);
    settings.setValue(name, value);
}
QVariant Config::ReadRegistry(const QString &position, const QString &name, const QVariant &value)
{
    QSettings settings(position, QSettings::NativeFormat);
    QVariant keyValue = settings.value(name, value);
    return keyValue;
}
#endif

QSize Config::GetDesktopSize() {
#ifdef __arm__
    int fb_fd = open("/dev/fb0", O_RDWR);
    int lcd_width, lcd_height;
    static struct fb_var_screeninfo var;

    if(-1 == fb_fd)
    {
        printf("cat't open /dev/fb0 \n");
        return QSize(0, 0);
    }
    //获取屏幕参数
    if(-1 == ioctl(fb_fd, FBIOGET_VSCREENINFO, &var))
    {
        close(fb_fd);
        printf("cat't ioctl /dev/fb0 \n");
        return QSize(0, 0);
    }

    // 计算参数
    lcd_width    = var.xres;
    lcd_height   = var.yres;

    printf("fb width:%d height:%d \n", lcd_width, lcd_height);
    close(fb_fd);

    return QSize(lcd_width, lcd_height);
#else
    return QSize(800, 480);
#endif
}
