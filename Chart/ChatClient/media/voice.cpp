#include "voice.h"
#include "myapp.h"
#include "unit.h"

#include <stdlib.h>
#include <stdio.h>
//#include <windows.h>
//#include <conio.h>
#include <errno.h>

#include <QDebug>
#include <QCoreApplication>
#include <QTime>
#include <QTextCodec>

#define UTF8(string)            QTextCodec::codecForName("UTF-8")->toUnicode((string))
#define UTF8_CHAR(string)       QTextCodec::codecForName("UTF-8")->fromUnicode((string)).constData()


/* 默认wav音频头部数据 */
wave_pcm_hdr default_wav_hdr =
{
    { 'R', 'I', 'F', 'F' },
    0,
    {'W', 'A', 'V', 'E'},
    {'f', 'm', 't', ' '},
    16,
    1,
    1,
    16000,
    32000,
    2,
    16,
    {'d', 'a', 't', 'a'},
    0
};


Voice::Voice(QObject *parent) :
    QObject(parent)
{

}

//延时
void Voice::sleep(int sec)
{
    QTime dieTime = QTime::currentTime().addMSecs(sec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

/*
* rdn:           合成音频数字发音方式
* volume:        合成音频的音量
* pitch:         合成音频的音调
* speed:         合成音频对应的语速
* voice_name:    合成发音人
* sample_rate:   合成音频采样率
* text_encoding: 合成文本编码格式
*
* 详细参数说明请参阅《iFlytek MSC Reference Manual》
*/
int Voice::changeTextToWav(s_wave_t wave_t)
{
    int         ret                  = MSP_SUCCESS;
    // 登录参数,appid与msc库绑定,请勿随意改动
//    const char* login_params         = "appid = 573c0f60, work_dir = .";
    QByteArray loginParames = "appid = 573c0f60, work_dir = ";
    loginParames.append(MyApp::m_strDataPath);

    //合成的语音文件名称
    QByteArray dataCfg = "voice_name = ";     // 播报人
    dataCfg.append(wave_t.speeker);
    dataCfg.append(", background_sound = ");
    dataCfg.append(QString::number(wave_t.bgs));               // 背景音
    dataCfg.append(", text_encoding = utf8");
    dataCfg.append(", sample_rate = ");
    dataCfg.append(wave_t.rate);
    dataCfg.append(", speed = ");
    dataCfg.append(QString::number(wave_t.speed));
    dataCfg.append(", volume = ");
    dataCfg.append(QString::number(wave_t.volume));
    dataCfg.append(", pitch = ");
    dataCfg.append(QString::number(wave_t.pitch));
    dataCfg.append(", rdn = ");
    dataCfg.append(QString::number(wave_t.rdn));

    // 文件名
    QString strFileName = wave_t.fileName;

    /* 用户登录 */
    ret = MSPLogin(NULL, NULL, loginParames.constData()); //第一个参数是用户名，第二个参数是密码，第三个参数是登录参数，用户名和密码可在http://open.voicecloud.cn注册获取

    if (MSP_SUCCESS != ret)
    {
        Q_EMIT signalStatus(VOICE_ERROR, QString("讯飞注册失败: %1").arg(ret));
        // 退出登录
        logout();
        return -1;
    }

    /* 文本合成 */
    Q_EMIT signalStatus(VOICE_CHANGING, "开始合成!");

    ret = text_to_speech(UTF8_CHAR(wave_t.text),
                         UTF8_CHAR(strFileName),
                         dataCfg.constData());

    if (MSP_SUCCESS != ret)
    {
        Q_EMIT signalStatus(VOICE_ERROR, QString("合成失败：文字转换失败: %1").arg(ret));
    }
    else {
        Q_EMIT signalStatus(VOICE_FINISHED, wave_t.fileName + ".wav");
    }


    // 退出登录
    logout();

    return 1;
}

void Voice::logout()
{
    //_getch();
    MSPLogout(); //退出登录
}


/* 文本合成 */
int Voice::text_to_speech(const char* src_text, const char* des_path, const char* params)
{
    int          ret          = -1;
    FILE*        fp           = NULL;
    const char*  sessionID    = NULL;
    unsigned int audio_len    = 0;
    wave_pcm_hdr wav_hdr      = default_wav_hdr;
    int          synth_status = MSP_TTS_FLAG_STILL_HAVE_DATA;

    if (NULL == src_text || NULL == des_path)
    {
        return ret;
    }
    fp = fopen(des_path, "wb");
    if (NULL == fp)
    {
        return ret;
    }
    /* 开始合成 */
    sessionID = QTTSSessionBegin(params, &ret);
    if (MSP_SUCCESS != ret)
    {
        fclose(fp);
        return ret;
    }
    ret = QTTSTextPut(sessionID, src_text, (unsigned int)strlen(src_text), NULL);
    if (MSP_SUCCESS != ret)
    {
        QTTSSessionEnd(sessionID, "TextPutError");
        fclose(fp);
        return ret;
    }
    fwrite(&wav_hdr, sizeof(wav_hdr) ,1, fp); //添加wav音频头，使用采样率为16000
    int nStep = 0;
    QString strStep = "";
    while (1)
    {
        /* 获取合成音频 */
        const void* data = QTTSAudioGet(sessionID, &audio_len, &synth_status, &ret);
        if (MSP_SUCCESS != ret)
            break;
        if (NULL != data)
        {
            fwrite(data, audio_len, 1, fp);
            wav_hdr.data_size += audio_len; //计算data_size大小
        }

        if (MSP_TTS_FLAG_DATA_END == synth_status)
            break;

        if (0 == nStep) {
            strStep = QString("当前进度: > ");
        }
        else {
            strStep += QString("> ");
        }
        Q_EMIT signalStatus(VOICE_CHANGING, strStep);

        nStep++;
        if (nStep > 10) nStep = 0;

        sleep(150); // 防止频繁占用CPU
    }
    //合成状态synth_status取值请参阅《讯飞语音云API文档》
    if (MSP_SUCCESS != ret)
    {
        QTTSSessionEnd(sessionID, "AudioGetError");
        fclose(fp);
        return ret;
    }

    /* 修正wav文件头数据的大小 */
    wav_hdr.size_8 += wav_hdr.data_size + (sizeof(wav_hdr) - 8);

    /* 将修正过的数据写回文件头部,音频文件为wav格式 */
    fseek(fp, 4, 0);
    fwrite(&wav_hdr.size_8,sizeof(wav_hdr.size_8), 1, fp); //写入size_8的值
    fseek(fp, 40, 0); //将文件指针偏移到存储data_size值的位置
    fwrite(&wav_hdr.data_size,sizeof(wav_hdr.data_size), 1, fp); //写入data_size的值
    fclose(fp);
    fp = NULL;

    /* 合成完毕 */
    ret = QTTSSessionEnd(sessionID, "Normal");

    return ret;
}

