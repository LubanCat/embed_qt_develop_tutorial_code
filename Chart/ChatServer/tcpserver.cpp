#include "tcpserver.h"
#include "clientsocket.h"
#include "unit.h"
#include "databasemagr.h"

#include <QHostAddress>

///////////////////////////////////////////////////////////////////////////////
/// \brief TcpMsgServer::TcpMsgServer
/// \param parent
///
TcpServer::TcpServer(QObject *parent) :
    QObject(parent)
{
    m_tcpServer = new QTcpServer(this);

    connect(m_tcpServer, SIGNAL(newConnection()), this, SLOT(SltNewConnection()));
}

TcpServer::~TcpServer()
{
    if (m_tcpServer->isListening()) m_tcpServer->close();
}

/**
 * @brief TcpServer::StartListen
 * 启动监听
 * @param port
 * @return
 */
bool TcpServer::StartListen(int port)
{
    if (m_tcpServer->isListening()) m_tcpServer->close();
    bool bOk = m_tcpServer->listen(QHostAddress::Any, port);
    return bOk;
}

/**
 * @brief TcpServer::CloseListen
 * 关闭
 */
void TcpServer::CloseListen()
{
    m_tcpServer->close();
}


///////////////////////////////////////////////////////////////////////////////
/// \brief TcpMsgServer::TcpMsgServer
/// \param parent
///
TcpMsgServer::TcpMsgServer(QObject *parent) :
    TcpServer(parent)
{
}

TcpMsgServer::~TcpMsgServer()
{
    qDebug() << "tcp server close";
    foreach (ClientSocket *client, m_clients) {
        m_clients.removeOne(client);
        client->Close();
    }
}

// 有新的客户端连接进来
void TcpMsgServer::SltNewConnection()
{
    ClientSocket *client = new ClientSocket(this, m_tcpServer->nextPendingConnection());
    connect(client, SIGNAL(signalConnected()), this, SLOT(SltConnected()));
    connect(client, SIGNAL(signalDisConnected()), this, SLOT(SltDisConnected()));
}

/**
 * @brief TcpMsgServer::SltConnected
 * 通过验证后，才可以加入容器进行管理
 */
void TcpMsgServer::SltConnected()
{
    ClientSocket *client = (ClientSocket *)this->sender();
    if (NULL == client) return;

    connect(client, SIGNAL(signalMsgToClient(quint8,int,QJsonValue)),
            this, SLOT(SltMsgToClient(quint8,int,QJsonValue)));
    connect(client, SIGNAL(signalDownloadFile(QJsonValue)), this, SIGNAL(signalDownloadFile(QJsonValue)));

    Q_EMIT signalUserStatus(QString("用户 [%1] 上线").arg(DataBaseMagr::Instance()->GetUserName(client->GetUserId())));
    m_clients.push_back(client);
}

/**
 * @brief TcpMsgServer::SltDisConnected
 * 有客户端下线
 */
void TcpMsgServer::SltDisConnected()
{
    ClientSocket *client = (ClientSocket *)this->sender();
    if (NULL == client) return;

    for (int i = 0; i < m_clients.size(); i++) {
        if (client == m_clients.at(i))
        {
            m_clients.remove(i);
            Q_EMIT signalUserStatus(QString("用户 [%1] 下线").arg(DataBaseMagr::Instance()->GetUserName(client->GetUserId())));
            return;
        }
    }

    disconnect(client, SIGNAL(signalConnected()), this, SLOT(SltConnected()));
    disconnect(client, SIGNAL(signalDisConnected()), this, SLOT(SltDisConnected()));
    disconnect(client, SIGNAL(signalMsgToClient(quint8,int,QJsonValue)),
               this, SLOT(SltMsgToClient(quint8,int,QJsonValue)));
    disconnect(client, SIGNAL(signalDownloadFile(QJsonValue)), this, SIGNAL(signalDownloadFile(QJsonValue)));
}

/**
 * @brief TcpMsgServer::SltMsgToClient
 * 消息转发控制
 * @param userId
 * @param msg
 */
void TcpMsgServer::SltMsgToClient(const quint8 &type, const int &id, const QJsonValue &json)
{
    // 查找要发送过去的id
    for (int i = 0; i < m_clients.size(); i++) {
        if (id == m_clients.at(i)->GetUserId())
        {
            m_clients.at(i)->SltSendMessage(type, json);
            return;
        }
    }
}

/**
 * @brief TcpMsgServer::SltTransFileToClient
 * @param userId
 * @param fileName
 */
void TcpMsgServer::SltTransFileToClient(const int &userId, const QJsonValue &json)
{
    // 查找要发送过去的id
    for (int i = 0; i < m_clients.size(); i++) {
        if (userId == m_clients.at(i)->GetUserId())
        {
            m_clients.at(i)->SltSendMessage(SendFile, json);
            return;
        }
    }
}


//////////////////////////////////////////////////////////////////
/// \brief TcpFileServer::TcpFileServer
/// \param parent
/// 文件中转服务器，客户端先把待转发的文件发送到服务器，服务器接受完成后，通知
/// 其他客户端来下载
TcpFileServer::TcpFileServer(QObject *parent) :
    TcpServer(parent)
{
}

TcpFileServer::~TcpFileServer()
{
    qDebug() << "tcp server close";
    foreach (ClientFileSocket *client, m_clients) {
        m_clients.removeOne(client);
        client->Close();
    }
}

void TcpFileServer::SltNewConnection()
{
    ClientFileSocket *client = new ClientFileSocket(this, m_tcpServer->nextPendingConnection());
    connect(client, SIGNAL(signalConnected()), this, SLOT(SltConnected()));
    connect(client, SIGNAL(signalDisConnected()), this, SLOT(SltDisConnected()));
}

/**
 * @brief TcpFileServer::SltConnected
 * socket管理
 */
void TcpFileServer::SltConnected()
{
    ClientFileSocket *client = (ClientFileSocket *)this->sender();
    if (NULL == client) return;

    m_clients.push_back(client);
}

/**
 * @brief TcpFileServer::SltDisConnected
 */
void TcpFileServer::SltDisConnected()
{
    ClientFileSocket *client = (ClientFileSocket *)this->sender();
    if (NULL == client) return;

    for (int i = 0; i < m_clients.size(); i++) {
        if (client == m_clients.at(i))
        {
            m_clients.remove(i);
            return;
        }
    }

    disconnect(client, SIGNAL(signalConnected()), this, SLOT(SltConnected()));
    disconnect(client, SIGNAL(signalDisConnected()), this, SLOT(SltDisConnected()));
}

/**
 * @brief TcpFileServer::SltClientDownloadFile
 * 客户端请求下载文件
 */
void TcpFileServer::SltClientDownloadFile(const QJsonValue &json)
{
    // 根据ID寻找连接的socket
    if (json.isObject()) {
        QJsonObject jsonObj = json.toObject();
        qint32 nId = jsonObj.value("from").toInt();
        qint32 nWid = jsonObj.value("id").toInt();;
        QString fileName = jsonObj.value("msg").toString();
        qDebug() << "get file" << jsonObj << m_clients.size();
        for (int i = 0; i < m_clients.size(); i++) {
            if (m_clients.at(i)->CheckUserId(nId, nWid))
            {
                m_clients.at(i)->StartTransferFile(fileName);
                return;
            }
        }
    }
}

