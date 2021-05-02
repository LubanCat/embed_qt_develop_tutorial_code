#include "testmedia.h"
#include <QDateTime>
#include "global.h"

TestMedia *TestMedia::self = 0;
TestMedia::TestMedia(QObject *parent) :
    QObject(parent)
{
    // 初始化播放函数
    initFmod();

    voiceSpech = new Voice(this);
    connect(voiceSpech, SIGNAL(signalStatus(E_VOICE_STATUS,QString)),
            this, SIGNAL(signalStatus(E_VOICE_STATUS,QString)));
}

/**
 * @brief TestMedia::textToWav
 * @param text
 * @param speeker
 * @param fileName
 * 文字转语音
 */
void TestMedia::textToWav(
        const QString &text,        // 要转换的文字
        int speeker,                // 语音播报员
        const QString &fileName)    // 保存的文件名字
{
    s_wave_t wave_t;
    //------------- 获取配置 -------------//
    QString strName = fileName;
    if (strName.isEmpty()) {
        strName = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
    }
    // 文件名
    wave_t.fileName = strName;
    // 编码配置
    wave_t.encod = "UTF8";
    // 配置文本
    wave_t.text = text;
    // 是否配置背景音乐
    wave_t.bgs = 0;
    // 采样率
    wave_t.rate = "16000";

    // 配置音调
    wave_t.pitch = 50;
    // 配置语速
    wave_t.speed = 50;
    // 配置合成音量
    wave_t.volume = 50;

    //  选择播音员
    switch (speeker) {
    case 0: wave_t.speeker = "xiaoyan"; break;
    case 1: wave_t.speeker = "xiaoqi"; break;
    case 2: wave_t.speeker = "xiaoyu"; break;
    case 3: wave_t.speeker = "xiaolaoma"; break;
    case 4: wave_t.speeker = "xiaomei"; break;
    case 5: wave_t.speeker = "xiaorong"; break;
    case 6: wave_t.speeker = "xiaoqiang"; break;
    case 7: wave_t.speeker = "henry"; break;
    case 8: wave_t.speeker = "mary"; break;
    default: wave_t.speeker = "xiaoyan"; break;
    }

    // 编码配置
    voiceSpech->changeTextToWav(wave_t);
}

/**
 * @brief TestMedia::playWav
 * @param fileName
 * 播放wav文件
 */
void TestMedia::playWav(const QString &strFileName)
{
    /*
    // 停止播放
    stopWav();

    FMOD_System_CreateSound(system, strFileName.toLocal8Bit().data(), FMOD_SOFTWARE | FMOD_CREATESAMPLE, 0, &sound);
    FMOD_Sound_SetMode(sound, FMOD_LOOP_OFF);
    FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, sound, false, &channel);
    */
}

/**
 * @brief TestMedia::initFmod
 */
void TestMedia::initFmod()
{
    FMOD_System_Create(&system);
    FMOD_System_Init(system, 32, FMOD_INIT_NORMAL, 0);
    sound = NULL;
    channel = NULL;
}

/**
 * @brief TestMedia::stopWav
 */
void TestMedia::stopWav()
{
    if (NULL != channel)
    {
        FMOD_Channel_Stop(channel);
        channel = NULL;
    }
}

// 状态
void TestMedia::sltShowStatus(E_VOICE_STATUS status_code, QString strStatus)
{
    if (VOICE_FINISHED == status_code) {
        qDebug() << "test change to wav finished!" << strStatus;
    }
    else {
//        myError("test change to wav failed!");
    }
}
