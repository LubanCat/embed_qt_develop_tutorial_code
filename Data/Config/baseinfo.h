#ifndef BASEINFO_H
#define BASEINFO_H

#include <QObject>
#include <QFile>
#include <QMutexLocker>
#include <QSharedPointer>
#include <QProcess>

#if defined(Q_OS_WIN32)
#include <windows.h>
#elif defined(Q_OS_LINUX)

#endif

class BaseInfo : public QObject
{
    Q_OBJECT
public:
    static QSharedPointer<BaseInfo>& instance()
    {
        if (self.isNull())
        {
            QMutexLocker mutexLocker(&m_Mutex);
            if (self.isNull())
                self = QSharedPointer<BaseInfo>(new BaseInfo());
        }
        return self;
    }
    ~BaseInfo();

private:
    BaseInfo(QObject *parent = nullptr);
    BaseInfo(const BaseInfo&){}
    BaseInfo& operator==(const BaseInfo&){}

private slots:
    void on_readoutput();
    void on_readerror();

private:
    static QMutex m_Mutex;
    static QSharedPointer<BaseInfo> self;

    QProcess *cmd;
    QString cmdback;


public:
    //CPU
    QString GetcpuName();
    QString GetcpuCoreNumber();
    QString GetcpuLogicalProcessors();
    QString GetcpuProcessorId();
    QString GetcpuBoardNumber();
    QString GetcpuBiosNumber();
    QString GetcpuDiskDrive();

#if defined(Q_OS_WIN32)
    //windows.h
    SYSTEM_INFO GetcpuInfo();
    MEMORYSTATUS GetmemoryStatus();
    MEMORYSTATUSEX GetmemoryStatusex();
    MEMORY_BASIC_INFORMATION GetMemoryBasic();
#elif defined(Q_OS_LINUX)

#endif

};
#endif // BASEINFO_H
