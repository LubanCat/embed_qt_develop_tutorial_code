#include "log.h"

#include <QApplication>
#include <QTextStream>
#include <QTextEdit>
#include <QDate>

#define QDATE qPrintable(QDate::currentDate().toString("yyyy-MM-dd"))

QMutex Log::m_Mutex;
QSharedPointer<Log> Log::self;

void mylog(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static QMutex mutex;
    QMutexLocker lock(&mutex);
    QString log;
    switch(type)
    {
    case QtDebugMsg:
        log = QString("%1").arg(msg);
        break;
    case QtWarningMsg:
        log = QString("%1").arg(msg);
        break;
    case QtCriticalMsg:
        log = QString("%1").arg(msg);
        break;
    case QtFatalMsg:
        log = QString("%1").arg(msg);
    }

    Log::instance()->SaveLog(log);
}

Log::Log(QObject *parent) : QObject(parent)
{
    //connect(this, SIGNAL(send(QString)), Log::instance(), SLOT(send(QString)));
    //logpath=qApp->applicationFilePath()+"/conf";

    file=new QFile(this);
    logpath="./conf";
    allow=false;
    logname="log";
}
void Log::InstallLog()
{
    qInstallMessageHandler(mylog);
}
void Log::UninstallLog()
{
    qInstallMessageHandler(nullptr);
}
void Log::SaveLog(QString log)
{
    logstr=log;

    if(allow)
        emit sendLog(log);

    QString logfile = QString("%1/%2_%3.txt").arg(logpath).arg(logname).arg(QDATE);
    if (this->logfile != logfile) {
        this->logfile = logfile;
        if (file->isOpen()) {
            file->close();
        }

        file->setFileName(logfile);
        file->open(QIODevice::WriteOnly | QIODevice::Append | QFile::Text);
    }

    QTextStream logStream(file);
    logStream << log << "\n";

}
void Log::SetAllowEmit(bool state)
{
    allow=state;
}
