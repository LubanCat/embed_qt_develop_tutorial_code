#ifndef MYAPP_H
#define MYAPP_H

#include <QObject>

class QApplication;

class MyApp
{
public:
    //=======================系统配置部分=========================//
    static QString m_strAppPath;         // 应用程序路径
    static QString m_strDataPath;        // 数据保存路径
    static QString m_strRecvPath;        // 音频目录
    static QString m_strDatabasePath;    // 数据库目录
    static QString m_strConfPath;        // 配置目录
    static QString m_strSoundPath;       // 配置目录
    static QString m_strRecordPath;      // 录音目录

    static QString m_strFacePath;        // 配置目录
    static QString m_strHeadPath;        // 配置目录

    static QString m_strIniFile;         // 配置文件

    static QString m_strHostAddr;       // 服务器地址配置
    static int     m_nMsgPort;          // 聊天消息服务器端口配置
    static int     m_nFilePort;          // 文件转发服务器端口配置
    static int     m_nGroupPort;        // 群组聊天窗口

    static QString m_strUserName;       // 用户名
    static QString m_strPassword;       // 用户密码
    static QString m_strHeadFile;       // 头像文件

    static int     m_nId;

    static int     m_nWinX;
    static int     m_nWinY;

    //=======================函数功能部分=========================//
    // 初始化
    static void InitApp(const QString &appPath);
    // 创建配置文件
    static void CreatorSettingFile();
    // 读配置文件，加载系统配置
    static void ReadSettingFile();

    // 读取/修改配置文件
    static void SetSettingFile(const QString &group, const QString &key, const QVariant &value);
    static QVariant GetSettingKeyValue(const QString &group, const QString &key, const QVariant &value);
    // 检查目录
    static void CheckDirs();
    static void CheckSound();

    static void SaveConfig();
};

#endif // MYAPP_H
