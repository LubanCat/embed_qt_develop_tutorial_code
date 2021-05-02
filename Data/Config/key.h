#ifndef KEYWIDGET_H
#define KEYWIDGET_H

#include <QObject>
#include <QFile>
#include <QMutexLocker>
#include <QSharedPointer>

/***************************************************************************
 *                          @brief 加密
 * 常见加密算法
 * MD5算法(摘要算法)  对一段信息产生信息摘要，以防止被篡改
 * SHA1算法 消息摘要算法，比MD5的安全性更强
 * Base64 是一种用64个字符来表示任意二进制数据的方法
 * 大小写加密
 * 异或加密
 * HMAC算法 运算利用哈希算法(MD5、SHA1 等)，以一个密钥 和一个消息为输入，生成一个消息摘要作为输出。
 * AES/DES/3DES算法 对称的块加密算法，加解密的过程是可逆的
 * DES算法 算法是一种分组密码，以64位为分组对数据加密，它的密钥长度是56位，加密解密用同一算法。
 * 3DES算法 是基于DES的对称算法，对一块数据用三个不同的密钥进行三次加密，强度更高。
 * AES算法 算法是密码学中的 高级加密标准，该加密算法采用 对称分组密码体制
 * RSA算法 RSA是第一个能同时用于加密和数字签名的算法，它能够抵抗到目前为止已知的所有密码攻击，已被ISO推荐为公钥数据加密标准
 * ECC算法 主要优势是在某些情况下，它比其他的方法使用更的密钥
 ***************************************************************************/
class EncryptionKEY : public QObject
{
    Q_OBJECT
public:
    static QSharedPointer<EncryptionKEY>& instance()
    {
        if (self.isNull())
        {
            QMutexLocker mutexLocker(&m_Mutex);
            if (self.isNull())
                self = QSharedPointer<EncryptionKEY>(new EncryptionKEY());
        }
        return self;
    }
private:
    EncryptionKEY(QObject *parent = nullptr);
    EncryptionKEY(const EncryptionKEY&){}
    EncryptionKEY& operator==(const EncryptionKEY&){}
private:
    static QMutex m_Mutex;
    static QSharedPointer<EncryptionKEY> self;

public:
    QString EncodeMD5(QString str);
    QString EncodeSHA1(QString str);
    QString EncodeBase64(QString str);
    QString EncodeUpprtANDLower(QString str);
    QString EncodeXOR(QString str, const QChar key);
};

/***************************************************************************
 *                          @brief 解密
 ***************************************************************************/
class DecryptKEY : public QObject
{
    Q_OBJECT
public:
    static QSharedPointer<DecryptKEY>& instance()
    {
        if (self.isNull())
        {
            QMutexLocker mutexLocker(&m_Mutex);
            if (self.isNull())
                self = QSharedPointer<DecryptKEY>(new DecryptKEY());
        }
        return self;
    }
private:
    DecryptKEY(QObject *parent = nullptr);
    DecryptKEY(const DecryptKEY&){}
    DecryptKEY& operator==(const DecryptKEY&){}
private:
    static QMutex m_Mutex;
    static QSharedPointer<DecryptKEY> self;

public:
    QString DecodeBase64(QString str);
    QString DecodeUpprtANDLower(QString str);
    QString DecodeXOR(QString str, const QChar key);
};

#endif // KEYWIDGET_H
