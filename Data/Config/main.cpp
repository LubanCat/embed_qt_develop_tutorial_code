#include "mainwindow.h"
#include <QApplication>

#include "config.h"
#include "log.h"
#include "runtime.h"
#include "baseinfo.h"

#include <QDebug>
#include <QDateTime>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Config::m_strSettingsFile = a.applicationDirPath() + "/conf/config.ini";

    //app 启动次数
    Config::instance()->SaveSetting("Etc", "runtimes",Config::instance()->ReadSetting("Etc", "runtimes").toInt()+1);

#if defined(Q_OS_WIN32)
    Config::instance()->SaveRegistry("HKEY_CURRENT_USER\\Software\\EBF_QT","APPNAME",QString("ebf_tutorial"));
    qDebug() <<  Config::instance()->ReadRegistry("HKEY_CURRENT_USER\\Software\\EBF_QT","APPNAME").toString();
#endif

    //Log::instance()->InstallLog();

    RunTime::instance()->RuntimeEnable(true);
    RunTime::instance()->SetRuntime(5000);

    BaseInfo::instance()->GetcpuName();

    QString msg = QString("当前时间: %1  %2").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")).arg("启动程序");
    qDebug() << msg;

    MainWindow w;
    w.show();

    return a.exec();
}
