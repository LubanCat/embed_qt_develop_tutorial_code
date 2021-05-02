#ifndef AUDIORECORDER_H
#define AUDIORECORDER_H

#include <QFile>
#include <QWidget>
#include <QPushButton>
#include <QAudioInput>
#include <QAudioOutput>
#include <QAudioDeviceInfo>
#include <QTimer>

class AudioRecorder : public QObject
{
	Q_OBJECT

public:
	AudioRecorder(QWidget *parent = 0);
	~AudioRecorder();

    void startRecord(QString strFileName);
    Q_SLOT void sltStopRecord();
    void saveRecordToFile(QString filename);
signals:
    void signalMaxValue(int val);
    void signalFinished();
private:
	int AddWavHeader(char *);
	int ApplyVolumeToSample(short iSample);
	void InitMonitor();
	void CreateAudioInput();
	void CreateAudioOutput();

private slots:
	void OnRecordPlay();
	void OnStateChange(QAudio::State s);
	void OnReadMore();
	void OnSliderValueChanged(int);
	void OnTimeOut();

private:
	int miVolume;
	int miMaxValue;

private:
	QAudioFormat mFormatFile;
	QFile *mpOutputFile;

	QAudioInput *mpAudioInputFile;		// 负责读写文件
	QAudioOutput *mpAudioOutputFile;

	QAudioFormat mFormatSound;
	QAudioInput *mpAudioInputSound;		// 负责监听声音
	QAudioOutput *mpAudioOutputSound;

	QIODevice *mpInputDevSound;
	QIODevice *mpOutputDevSound;
	QByteArray mBuffer;

    QTimer *m_timer;
    QString m_strFilename;
};

#endif // AUDIORECORDER_H
