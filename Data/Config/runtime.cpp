#include "runtime.h"
#include "config.h"

#include <QDate>
#include <QFile>
#include <QTextStream>

#include <QDebug>
#include <QDateTime>

#define QDATETIME_NOW qPrintable(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))

QMutex RunTime::m_Mutex;
QSharedPointer<RunTime> RunTime::self;

RunTime::RunTime(QObject *parent) : QObject(parent)
{
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this, SLOT(SaveRuntime()));
    timer->setInterval(1000);

    runtimepath="./conf";
    runtimename="runtime";
    times = Config::instance()->ReadSetting("Etc", "runtimes").toInt();
    Config::instance()->SaveSetting("Etc", "starttime", QDATETIME_NOW);

    QString filestr = QString("%1/%2_%3.txt").arg(runtimepath).arg(runtimename).arg(QDate::currentDate().year());
    QFile file(filestr);
    if (file.size() == 0)
    {
        CreateRuntimeFile();
    }
    AppendRuntime();
}
void RunTime::RuntimeEnable(bool state)
{
    if(state)
        timer->start();
    else
        timer->stop();
}
void RunTime::SetRuntime(int msec)
{
    timer->setInterval(msec);
}

void RunTime::CreateRuntimeFile()
{
    QString filestr = QString("%1/%2_%3.txt").arg(runtimepath).arg(runtimename).arg(QDate::currentDate().year());
    QFile file(filestr);

    if (file.open(QFile::WriteOnly | QFile::Text))
    {
        QTextStream filestream(&file);

        QString str = QString("%1\t%2\t\t%3\t\t%4\r\n").arg("runtimes").arg("starttime").arg("endtime").arg("runtime");
        filestream << str;
        file.close();
    }
}

void RunTime::AppendRuntime()
{
    QString filestr = QString("%1/%2_%3.txt").arg(runtimepath).arg(runtimename).arg(QDate::currentDate().year());
    QFile file(filestr);

    if (file.open(QFile::WriteOnly | QFile::Append | QFile::Text))
    {
        QTextStream filestream(&file);

        QString str = QString("%1\t%2\t\t%3\t\t%4\r\n").arg(times).arg(QDATETIME_NOW).arg(QDATETIME_NOW).arg(0);
        filestream << str;
        file.close();
    }
}

void RunTime::SaveRuntime()
{
    QString filestr = QString("%1/%2_%3.txt").arg(runtimepath).arg(runtimename).arg(QDate::currentDate().year());
    QFile file(filestr);

    if (file.size() == 0)
    {
        CreateRuntimeFile();
        AppendRuntime();
        return;
    }

    if (file.open(QFile::ReadWrite))
    {
        QStringList runtimelog;
        while (!file.atEnd()) {
            runtimelog.append(file.readLine());
        }

        QString lastline=runtimelog.last();
        QStringList list=lastline.split("\t");

        if(list.count()>5)
        {
            list[3]=QDATETIME_NOW;
            QDateTime endtime=QDateTime::fromString(list[3],"yyyy-MM-dd hh:mm:ss");
            QDateTime starttime=QDateTime::fromString(list[1],"yyyy-MM-dd hh:mm:ss");
            qint64 runtime_s = starttime.secsTo(endtime);
            list[5]=QString::number(runtime_s);
            QString str=list.join("\t")+"\r\n";

            runtimelog[runtimelog.count() - 1] = str;
            file.resize(0);
            QTextStream filestream(&file);
            filestream << runtimelog.join("");
            file.close();
        }
    }
}
