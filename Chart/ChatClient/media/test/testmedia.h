#ifndef TESTMEDIA_H
#define TESTMEDIA_H

#include <QObject>
#include <QMutex>
#include "voice.h"
#include "fmod.h"
#include "fmod_errors.h"

class TestMedia : public QObject
{
    Q_OBJECT
public:
    explicit TestMedia(QObject *parent = 0);

    static TestMedia *Instance()
    {
        static QMutex mutex;

        if (!self) {
            QMutexLocker locker(&mutex);

            if (!self) {
                self = new TestMedia;
            }
        }

        return self;
    }

    void textToWav(const QString &text, int speeker, const QString &fileName);
    void playWav(const QString &strFileName);
signals:
    void signalStatus(E_VOICE_STATUS,QString);
public slots:

private:
    Voice *voiceSpech;
    static TestMedia *self;

    FMOD_SYSTEM *system;
    FMOD_SOUND *sound;
    FMOD_CHANNEL *channel;

    void initFmod();

    void stopWav();
private slots:
    void sltShowStatus(E_VOICE_STATUS status_code, QString strStatus);
};

#endif // TESTMEDIA_H
