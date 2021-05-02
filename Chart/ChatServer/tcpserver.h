#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include <QVector>

#include "clientsocket.h"

//////////////////////////////////////////////////////////////////////
/// \brief The TcpServer class
/// 服务器管理类
class TcpServer : public QObject {
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = 0);
    ~TcpServer();

    bool StartListen(int port = 6666);
    void CloseListen();
signals:
    void signalUserStatus(const QString &text);
protected:
    QTcpServer *m_tcpServer;

public slots:

protected slots:
    // 继承虚函数
    virtual void SltNewConnection() = 0;
    virtual void SltConnected() = 0;
    virtual void SltDisConnected() = 0;
};

///////////////////////////////////////////////////////
/// \brief The TcpMsgServer class
/// 消息服务器
class TcpMsgServer : public TcpServer
{
    Q_OBJECT
public:
    explicit TcpMsgServer(QObject *parent = 0);
    ~TcpMsgServer();

signals:
    void signalDownloadFile(const QJsonValue &json);

private:
    // 客户端管理
    QVector < ClientSocket * > m_clients;
public slots:
    void SltTransFileToClient(const int &userId, const QJsonValue &json);

private slots:
    void SltNewConnection();
    void SltConnected();
    void SltDisConnected();
    void SltMsgToClient(const quint8 &type, const int &id, const QJsonValue &json);
};

////////////////////////////////////////////////////////////////
/// \brief The TcpFileServer class
/// 文件服务器
class TcpFileServer : public TcpServer {
    Q_OBJECT
public :
    explicit TcpFileServer(QObject *parent = 0);
    ~TcpFileServer();
signals:
    void signalRecvFinished(int id, const QJsonValue &json);
private:
    // 客户端管理
    QVector < ClientFileSocket * > m_clients;

private slots:
    void SltNewConnection();
    void SltConnected();
    void SltDisConnected();
    void SltClientDownloadFile(const QJsonValue &json);
};

#endif // TCPSERVER_H
