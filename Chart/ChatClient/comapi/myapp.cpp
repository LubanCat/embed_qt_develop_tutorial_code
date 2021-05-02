#include "myapp.h"
#include "unit.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QSettings>
#include <QFile>
#include <QDir>
#include <QProcess>
#include <QDebug>

// 应用程序配置目录
QString MyApp::m_strAppPath         = "./";
QString MyApp::m_strDataPath        = "";
QString MyApp::m_strRecvPath        = "";
QString MyApp::m_strDatabasePath    = "";
QString MyApp::m_strConfPath        = "";
QString MyApp::m_strFacePath        = "";
QString MyApp::m_strHeadPath        = "";
QString MyApp::m_strSoundPath       = "";
QString MyApp::m_strRecordPath      = "";
// 配置文件
QString MyApp::m_strIniFile         = "config.ini";

#if 0
// 服务器相关配置
QString MyApp::m_strHostAddr        = "183.230.8.172";
int     MyApp::m_nMsgPort           = 32101;
int     MyApp::m_nFilePort          = 32102;
int     MyApp::m_nGroupPort         = 32103;
#else
// 服务器相关配置
QString MyApp::m_strHostAddr        = "192.168.5.181";
int     MyApp::m_nMsgPort           = 60001;
int     MyApp::m_nFilePort          = 60002;
int     MyApp::m_nGroupPort         = 60003;
#endif
QString MyApp::m_strUserName        = "zhangsan";
QString MyApp::m_strPassword        = "123456";
QString MyApp::m_strHeadFile        = "head-64.png";

int     MyApp::m_nId                = -1;
int     MyApp::m_nWinX              = 0;
int     MyApp::m_nWinY              = 0;

// 初始化
void MyApp::InitApp(const QString &appPath)
{
    m_strAppPath        = appPath + "/";

    m_strDataPath       = m_strAppPath  + "Data/";
    m_strRecvPath       = m_strDataPath + "RecvFiles/";
    m_strDatabasePath   = m_strDataPath + "Database/";
    m_strConfPath       = m_strDataPath + "Conf/";
    m_strHeadPath       = m_strDataPath + "Head/";
    m_strSoundPath      = m_strDataPath + "Sound/";
    m_strRecordPath     = m_strDataPath + "Record/";
    m_strFacePath       = m_strDataPath + "Face/";
    m_strIniFile        = m_strConfPath + "config.ini";

    // 检查目录
    CheckDirs();

    // 检测音频文件
    CheckSound();

    // 创建配置文件
    CreatorSettingFile();

    // 加载系统配置
    ReadSettingFile();
}

/**
 * @brief MyApp::creatorSettingFile 创建配置文件
 */
void MyApp::CreatorSettingFile() {
    // 写入配置文件
    QSettings settings(m_strIniFile, QSettings::IniFormat);
    QString strGroups = settings.childGroups().join("");
    if (!QFile::exists(m_strIniFile) || (strGroups.isEmpty()))
    {

        /*系统设置*/
        settings.beginGroup("UserCfg");
        settings.setValue("User",   m_strUserName);
        settings.setValue("Passwd", m_strPassword);
        settings.endGroup();

        /*其他配置*/
        settings.beginGroup("Server");
        settings.setValue("HostAddr", m_strHostAddr);
        settings.setValue("MsgPort",  m_nMsgPort);
        settings.setValue("FilePort",  m_nFilePort);
        settings.setValue("GroupPort",  m_nGroupPort);
        settings.endGroup();
        settings.sync();

    }
#ifdef Q_WS_QWS
    QProcess::execute("sync");
#endif
}

/**
 * @brief MyApp::ReadSettingFile
 * 读取系统配置信息
 */
void MyApp::ReadSettingFile()
{
    QSettings settings(m_strIniFile, QSettings::IniFormat);
    settings.beginGroup("UserCfg");
    m_strUserName = settings.value("User", "zhangsan").toString();
    m_strPassword = settings.value("Passwd", "123456")  .toString();
    settings.endGroup();


    settings.beginGroup("Server");
    m_strHostAddr = settings.value("HostAddr", "183.230.8.172").toString();
    m_nMsgPort    = settings.value("MsgPort", 32101)  .toInt();
    m_nFilePort   = settings.value("FilePort", 32102)  .toInt();
    m_nGroupPort  = settings.value("GroupPort", 32103)  .toInt();
    settings.endGroup();
}

/**
 * @brief MyApp::setSettingFile 写入配置文件
 * @param group                 组
 * @param key                   key
 * @param value                 值
 */
void MyApp::SetSettingFile(const QString &group, const QString &key, const QVariant &value)
{
    QSettings settings(m_strIniFile, QSettings::IniFormat);
    settings.beginGroup(group);
    settings.setValue(key, value);
    settings.sync();
}

/**
 * @brief MyApp::getSettingKeyValue 读取配置参数
 * @param group
 * @param key
 * @return
 */
QVariant MyApp::GetSettingKeyValue(const QString &group, const QString &key, const QVariant &value)
{
    QSettings settings(m_strIniFile, QSettings::IniFormat);
    settings.beginGroup(group);
    return settings.value(key, value);
}

/**
 * @brief MyApp::checkDataDir 检查数据存储目录
 */
void MyApp::CheckDirs()
{
    // 数据文件夹
    QDir dir(m_strDataPath);
    if (!dir.exists()) {
        dir.mkdir(m_strDataPath);
#ifdef Q_WS_QWS
        QProcess::execute("sync");
#endif
    }


    // 接收文件目录
    dir.setPath(m_strRecvPath);
    if (!dir.exists()) {
        dir.mkdir(m_strRecvPath);
#ifdef Q_WS_QWS
        QProcess::execute("sync");
#endif
    }

    // 数据库目录
    dir.setPath(m_strDatabasePath);
    if (!dir.exists()) {
        dir.mkdir(m_strDatabasePath);
#ifdef Q_WS_QWS
        QProcess::execute("sync");
#endif
    }

    // 配置文件目录
    dir.setPath(m_strConfPath);
    if (!dir.exists()) {
        dir.mkdir(m_strConfPath);
#ifdef Q_WS_QWS
        QProcess::execute("sync");
#endif
    }

    // 表情目录
    dir.setPath(m_strFacePath);
    if (!dir.exists()) {
        dir.mkdir(m_strFacePath);
#ifdef Q_WS_QWS
        QProcess::execute("sync");
#endif
    }

    // 头像检测目录
    dir.setPath(m_strHeadPath);
    if (!dir.exists()) {
        dir.mkdir(m_strHeadPath);
#ifdef Q_WS_QWS
        QProcess::execute("sync");
#endif
    }

    // 音频目录
    dir.setPath(m_strSoundPath);
    if (!dir.exists()) {
        dir.mkdir(m_strSoundPath);
#ifdef Q_WS_QWS
        QProcess::execute("sync");
#endif
    }
}

/**
 * @brief MyApp::CheckSound
 * 音频文件检测
 */
void MyApp::CheckSound()
{
    if (!QFile::exists(MyApp::m_strSoundPath + "message.wav")) {
        QFile::copy(":/sound/resource/sound/message.wav", MyApp::m_strSoundPath + "message.wav");
    }

    if (!QFile::exists(MyApp::m_strSoundPath + "msg.wav")) {
        QFile::copy(":/sound/resource/sound/msg.wav", MyApp::m_strSoundPath + "msg.wav");
    }

    if (!QFile::exists(MyApp::m_strSoundPath + "ringin.wav")) {
        QFile::copy(":/sound/resource/sound/ringin.wav", MyApp::m_strSoundPath + "ringin.wav");
    }

    if (!QFile::exists(MyApp::m_strSoundPath + "system.wav")) {
        QFile::copy(":/sound/resource/sound/system.wav", MyApp::m_strSoundPath + "system.wav");
    }

    if (!QFile::exists(MyApp::m_strSoundPath + "userlogon.wav")) {
        QFile::copy(":/sound/resource/sound/userlogon.wav", MyApp::m_strSoundPath + "userlogon.wav");
    }
}

// 保存配置
void MyApp::SaveConfig()
{
    QSettings settings(m_strIniFile, QSettings::IniFormat);

    /*系统设置*/
    settings.beginGroup("UserCfg");
    settings.setValue("User",   m_strUserName);
    settings.setValue("Passwd", m_strPassword);
    settings.endGroup();

    /*其他配置*/
    settings.beginGroup("Server");
    settings.setValue("HostAddr", m_strHostAddr);
    settings.setValue("MsgPort",  m_nMsgPort);
    settings.setValue("FilePort",  m_nFilePort);
    settings.setValue("GroupPort",  m_nGroupPort);
    settings.endGroup();
    settings.sync();
}
