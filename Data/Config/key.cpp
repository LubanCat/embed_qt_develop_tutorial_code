#include "key.h"

#include <QDebug>
#include <QCryptographicHash>

#define QDATETIME_NOW qPrintable(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))

QMutex EncryptionKEY::m_Mutex;
QSharedPointer<EncryptionKEY> EncryptionKEY::self;
QMutex DecryptKEY::m_Mutex;
QSharedPointer<DecryptKEY> DecryptKEY::self;

/***************************************************************************
 *                          @brief 加密
 ***************************************************************************/
EncryptionKEY::EncryptionKEY(QObject *parent) : QObject(parent)
{

}

QString EncryptionKEY::EncodeMD5(QString str)
{
    return QCryptographicHash::hash(str.toLatin1(),QCryptographicHash::Md5).toHex();
}
QString EncryptionKEY::EncodeSHA1(QString str)
{
    return QCryptographicHash::hash(str.toLatin1(),QCryptographicHash::Sha1).toHex();
}
QString EncryptionKEY::EncodeBase64(QString str)
{
    return str.toLocal8Bit().toBase64();
}
QString EncryptionKEY::EncodeUpprtANDLower(QString str)
{
    for(int i=0; i<str.count();i++)
    {
        if(str.at(i).isUpper())
            str[i]=str.at(i).toLower();
        else if(str.at(i).isLower())
            str[i]=str.at(i).toUpper();
    }
    return str;
}
QString EncryptionKEY::EncodeXOR(QString str, const QChar key)
{
    for(int i = 0; i < str.count(); i++) {
        str[i] = str.at(i).toLatin1() ^ key.toLatin1();
    }
    return str;
}

/***************************************************************************
 *                          @brief 解密
 ***************************************************************************/
DecryptKEY::DecryptKEY(QObject *parent) : QObject(parent)
{

}
QString DecryptKEY::DecodeBase64(QString str)
{
    QByteArray temp = str.toLocal8Bit();
    return QString::fromLocal8Bit(temp.fromBase64(temp));
}
QString DecryptKEY::DecodeUpprtANDLower(QString str)
{
    for(int i=0; i<str.count();i++)
    {
        if(str.at(i).isUpper())
            str[i]=str.at(i).toLower();
        else if(str.at(i).isLower())
            str[i]=str.at(i).toUpper();
    }
    return str;
}
QString DecryptKEY::DecodeXOR(QString str, const QChar key)
{
    for(int i = 0; i < str.count(); i++) {
        str[i] = str.at(i).toLatin1() ^ key.toLatin1();
    }
    return str;
}
