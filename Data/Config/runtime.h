#ifndef RUNTIMEWIDGET_H
#define RUNTIMEWIDGET_H

#include <QObject>
#include <QFile>
#include <QMutexLocker>
#include <QSharedPointer>
#include <QTimer>

class RunTime : public QObject
{
    Q_OBJECT
public:
    static QSharedPointer<RunTime>& instance()
    {
        if (self.isNull())
        {
            QMutexLocker mutexLocker(&m_Mutex);
            if (self.isNull())
                self = QSharedPointer<RunTime>(new RunTime());
        }
        return self;
    }
private:
    RunTime(QObject *parent = nullptr);
    RunTime(const RunTime&){}
    RunTime& operator==(const RunTime&){}
public:
    void RuntimeEnable(bool state);
    void SetRuntime(int msec);

    void CreateRuntimeFile();
    void AppendRuntime();
signals:

private slots:

    void SaveRuntime();

public:
    QString logstr;
private:
    int times;

    QString runtimefile;
    QString runtimepath;
    QString runtimename;

    QTimer *timer;

    static QMutex m_Mutex;
    static QSharedPointer<RunTime> self;
};


#endif // RUNTIMEWIDGET_H
