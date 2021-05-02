#ifndef VOICE_H
#define VOICE_H

#include <QObject>

#include "qtts.h"
#include "msp_cmn.h"
#include "msp_errors.h"


typedef struct {
    QString fileName;       // 文件保存路径
    QString text;           // 要转换的文字
    QString encod;          // 要转换的文字编码
    QString rate;           // 采样速率
    short int bgs;          // 背景音： 0 无，1 有
    short int rdn;          // 数字发音
    short int speed;        // 播报语速
    short int volume;       // 合成音频的音量
    short int pitch;        // 合成音频的音调
    QString speeker;        // 语音播报人
} s_wave_t;

/* wav音频头部格式 */
typedef struct _wave_pcm_hdr
{
    char            riff[4];                // = "RIFF"
    int				size_8;                 // = FileSize - 8
    char            wave[4];                // = "WAVE"
    char            fmt[4];                 // = "fmt "
    int				fmt_size;				// = 下一个结构体的大小 : 16

    short int       format_tag;             // = PCM : 1
    short int       channels;               // = 通道数 : 1
    int				samples_per_sec;        // = 采样率 : 8000 | 6000 | 11025 | 16000
    int				avg_bytes_per_sec;      // = 每秒字节数 : samples_per_sec * bits_per_sample / 8
    short int       block_align;            // = 每采样点字节数 : wBitsPerSample / 8
    short int       bits_per_sample;        // = 量化比特数: 8 | 16

    char            data[4];                // = "data";
    int				data_size;              // = 纯数据长度 : FileSize - 44
} wave_pcm_hdr;


typedef enum {
    VOICE_FINISHED,     // 已经完成
    VOICE_CHANGING,     // 转换中
    VOICE_ERROR,        // 转换发生错误
} E_VOICE_STATUS;


class Voice : public QObject
{
    Q_OBJECT
public:
    Voice(QObject *parent = 0);

    int changeTextToWav(s_wave_t wave_t);
signals:
    void signalStatus(E_VOICE_STATUS status, QString StrStatus);
private:
    void logout();
    int text_to_speech(const char *src_text, const char *des_path, const char *params);
    void sleep(int sec);
};

#endif // VOICE_H
