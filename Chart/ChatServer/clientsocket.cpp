#include "clientsocket.h"
#include "databasemagr.h"
#include "unit.h"
#include "myapp.h"

#include <QDebug>
#include <QDataStream>
#include <QApplication>
#include <QFileInfo>

ClientSocket::ClientSocket(QObject *parent, QTcpSocket *tcpSocket) :
    QObject(parent)
{
    m_nId = -1;

    if (tcpSocket == NULL) m_tcpSocket = new QTcpSocket(this);
    m_tcpSocket = tcpSocket;

    connect(m_tcpSocket, SIGNAL(readyRead()), this, SLOT(SltReadyRead()));
    connect(m_tcpSocket, SIGNAL(connected()), this, SLOT(SltConnected()));
    connect(m_tcpSocket, SIGNAL(disconnected()), this, SLOT(SltDisconnected()));
}

ClientSocket::~ClientSocket()
{

}

int ClientSocket::GetUserId() const
{
    return m_nId;
}

void ClientSocket::Close()
{
    m_tcpSocket->abort();
}

void ClientSocket::SltConnected()
{
    qDebug() << "connected";
    //    Q_EMIT signalConnected();
}

void ClientSocket::SltDisconnected()
{
    qDebug() << "disconnected";
    DataBaseMagr::Instance()->UpdateUserStatus(m_nId, OffLine);
    Q_EMIT signalDisConnected();
}

/**
 * @brief ClientSocket::SltReadyRead
 * 读取socket数据
 */
void ClientSocket::SltReadyRead()
{
    // 读取socket数据
    QByteArray reply = m_tcpSocket->readAll();
    QJsonParseError jsonError;
    // 转化为 JSON 文档
    QJsonDocument doucment = QJsonDocument::fromJson(reply, &jsonError);
    // 解析未发生错误
    if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError)) {
        // JSON 文档为对象
        if (doucment.isObject()) {
            // 转化为对象
            QJsonObject jsonObj = doucment.object();
            int nType = jsonObj.value("type").toInt();
            QJsonValue dataVal = jsonObj.value("data");

            switch (nType) {
            case Register:
            {
                ParseReister(dataVal);
            }
                break;
            case Login:
            {
                ParseLogin(dataVal);
            }
                break;
            case UserOnLine:
            {
                ParseUserOnline(dataVal);
            }
                break;
            case Logout:
            {
                ParseLogout(dataVal);
                Q_EMIT signalDisConnected();
                m_tcpSocket->abort();
            }
                break;
            case UpdateHeadPic:
            {
                ParseUpdateUserHead(dataVal);
            }
                break;
            case AddFriend:
            {
                ParseAddFriend(dataVal);
            }
                break;
            case AddGroup:
            {
                ParseAddGroup(dataVal);
            }
                break;
            case CreateGroup:
            {
                ParseCreateGroup(dataVal);
            }
                break;
            case GetMyFriends:
            {
                ParseGetMyFriend(dataVal);
            }
                break;
            case GetMyGroups:
            {
                ParseGetMyGroups(dataVal);
            }
                break;

            case RefreshFriends:
            {
                ParseRefreshFriend(dataVal);
            }
                break;
            case RefreshGroups:
            {
                ParseRefreshGroups(dataVal);
            }
                break;

            case SendMsg:
            case SendFile:
            case SendPicture:
            {
                ParseFriendMessages(reply);
            }
                break;
            case SendGroupMsg:
            {
                ParseGroupMessages(reply);
            }
                break;
            case SendFace:
            {
                ParseGroupMessages(reply);
            }
                break;
            case SendFileOk:
            {

            }
                break;
            case GetFile:
            {
                Q_EMIT signalDownloadFile(dataVal);
            }
                break;
            default:
                break;
            }
        }
    }
}

/**
 * @brief ClientSocket::ParseLogin
 * 解析登录信息
 * @param reply
 */
void ClientSocket::ParseLogin(const QJsonValue &dataVal)
{
    // data 的 value 是对象
    if (dataVal.isObject()) {
        QJsonObject dataObj = dataVal.toObject();
        QString strName = dataObj.value("name").toString();
        QString strPwd = dataObj.value("passwd").toString();
        QJsonObject jsonObj = DataBaseMagr::Instance()->CheckUserLogin(strName, strPwd);

        m_nId = jsonObj.value("id").toInt();
        qDebug() << "login" << jsonObj;

        if (m_nId > 0) Q_EMIT signalConnected();
        // 发送查询结果至客户端
        SltSendMessage(Login, jsonObj);;
    }
}

/**
 * @brief ClientSocket::ParseUserOnline
 * 用户上线
 * @param dataVal
 */
void ClientSocket::ParseUserOnline(const QJsonValue &dataVal)
{
    // data 的 value 是数组
    if (dataVal.isArray()) {
        QJsonArray jsonArray = dataVal.toArray();
        int nSize = jsonArray.size();
        for (int i = 0; i < nSize; ++i) {
            int nId = jsonArray.at(i).toInt();
            int nStatus = DataBaseMagr::Instance()->GetUserLineStatus(nId);
            // 给在线的好友通报一下状态
            if (OnLine == nStatus) {
                QJsonObject jsonObj;
                jsonObj.insert("id", m_nId);
                jsonObj.insert("text", "online");

                Q_EMIT signalMsgToClient(UserOnLine, nId, jsonObj);
            }
        }
    }

}

/**
 * @brief ClientSocket::ParseLogin
 * 解析登录信息
 * @param reply
 */
void ClientSocket::ParseLogout(const QJsonValue &dataVal)
{
    // data 的 value 是对象
    if (dataVal.isObject()) {
        QJsonObject dataObj = dataVal.toObject();

        QJsonArray array = dataObj.value("friends").toArray();
        int nId = dataObj.value("id").toInt();
        int nSize = array.size();

        DataBaseMagr::Instance()->UpdateUserStatus(nId, OffLine);

        for (int i = 0; i < nSize; ++i) {
            nId = array.at(i).toInt();
            int nStatus = DataBaseMagr::Instance()->GetUserLineStatus(nId);
            // 给在线的好友通报一下状态
            if (OnLine == nStatus) {
                QJsonObject jsonObj;
                jsonObj.insert("id", m_nId);
                jsonObj.insert("text", "offline");

                Q_EMIT signalMsgToClient(UserOffLine, nId, jsonObj);
            }
        }
    }
}

/**
 * @brief ClientSocket::ParseUpdateUserHead
 * 更新用户头像文件
 * @param dataVal
 */
void ClientSocket::ParseUpdateUserHead(const QJsonValue &dataVal)
{
    // data 的 value 是对象
    if (dataVal.isObject()) {
        QJsonObject dataObj = dataVal.toObject();
        int nId = dataObj.value("id").toInt();
        QString strHead = dataObj.value("head").toString();

        // 更新数据库
        DataBaseMagr::Instance()->UpdateUserHead(nId, strHead);

        // 通知其他在线好友，说我已经修改了头像
        QJsonArray jsonFriends =  dataObj.value("friends").toArray();
        QJsonObject jsonObj;
        // 是我在更新，我要去下载我的头像
        jsonObj.insert("id", nId);
        jsonObj.insert("head", strHead);

        for (int i = 0; i < jsonFriends.size(); i++) {
            nId = jsonFriends.at(i).toInt();
            Q_EMIT signalMsgToClient(UpdateHeadPic, nId, jsonObj);
        }
    }
}

/**
 * @brief ClientSocket::ParseReister
 * 解析注册信息
 * @param reply
 */
void ClientSocket::ParseReister(const QJsonValue &dataVal)
{
    // data 的 value 是对象
    if (dataVal.isObject()) {
        QJsonObject dataObj = dataVal.toObject();
        QString strName = dataObj.value("name").toString();
        QString strPwd = dataObj.value("passwd").toString();
        m_nId = DataBaseMagr::Instance()->RegisterUser(strName, strPwd);

        // 返回客户端
        QJsonObject json;
        json.insert("id", m_nId);
        json.insert("name", strName);
        json.insert("passwd", strPwd);

        // 发送查询结果至客户端
        SltSendMessage(Register, json);;
    }
}

/**
 * @brief ClientSocket::ParseAddFriend
 * 添加好友
 * @param reply
 */
void ClientSocket::ParseAddFriend(const QJsonValue &dataVal)
{
    // data 的 value 是对象
    if (dataVal.isObject()) {

        QJsonObject dataObj = dataVal.toObject();
        QString strName = dataObj.value("name").toString();
        QJsonObject jsonFriend = DataBaseMagr::Instance()->AddFriend(strName);
        int nId = jsonFriend.value("id").toInt();
        QString strMsg = jsonFriend.value("msg").toString();

        // 发送查询结果至客户端
        SltSendMessage(AddFriend, jsonFriend);

        if (nId < 0) return;

        // 给对方ID发送add请求
        QJsonObject jsonQuery = DataBaseMagr::Instance()->GetUserInfo(m_nId);
        QJsonObject jsonRequest;
        jsonRequest.insert("id", m_nId);
        jsonRequest.insert("name", jsonQuery.value("name").toString());
        jsonRequest.insert("head", jsonQuery.value("head").toString());
        jsonRequest.insert("msg", strMsg.isEmpty() ? "附加消息： 你好！" : strMsg);

        Q_EMIT signalMsgToClient(AddFriendRequist, nId, jsonRequest);
    }
}

/**
 * @brief ClientSocket::ParseAddGroup
 * 添加群组
 * @param dataVal
 */
void ClientSocket::ParseAddGroup(const QJsonValue &dataVal)
{
    // data 的 value 是对象
    if (dataVal.isObject()) {
        QJsonObject dataObj = dataVal.toObject();
        int nId = dataObj.value("id").toInt();
        QString strName = dataObj.value("name").toString();
        QJsonObject json = DataBaseMagr::Instance()->AddGroup(nId, strName);

        // 发送查询结果至客户端
        SltSendMessage(AddGroup, json);
    }
}

/**
 * @brief ClientSocket::ParseCreateGroup
 * 创建群组
 * @param dataVal
 */
void ClientSocket::ParseCreateGroup(const QJsonValue &dataVal)
{
    // data 的 value 是对象
    if (dataVal.isObject()) {
        QJsonObject dataObj = dataVal.toObject();
        int nId = dataObj.value("id").toInt();
        QString strName = dataObj.value("name").toString();
        QJsonObject json = DataBaseMagr::Instance()->CreateGroup(nId, strName);

        // 发送查询结果至客户端
        SltSendMessage(CreateGroup, json);
    }
}

/**
 * @brief ClientSocket::ParseGetMyFriend
 * 上线的时候获取我的好友
 * @param dataVal
 */
void ClientSocket::ParseGetMyFriend(const QJsonValue &dataVal)
{
    QJsonArray jsonArray;
    // data 的 value 是数组
    if (dataVal.isArray()) {
        QJsonArray array = dataVal.toArray();
        int nSize = array.size();
        // 先添加在线好友，在添加离线好友
        for (int i = 0; i < nSize; ++i) {
            int nId = array.at(i).toInt();
            jsonArray.append(DataBaseMagr::Instance()->GetUserStatus(nId));
        }
    }

    this->SltSendMessage(GetMyFriends, jsonArray);
}

/**
 * @brief ClientSocket::ParseGetMyGroups
 * 上线的时候获取我的群组
 * @param dataVal
 */
void ClientSocket::ParseGetMyGroups(const QJsonValue &dataVal)
{
    QJsonArray jsonArray;
    // data 的 value 是数组
    if (dataVal.isObject()) {
        QJsonObject jsonObj = dataVal.toObject();
        // 群组ID
        int nId = jsonObj.value("id").toInt();
        jsonArray = DataBaseMagr::Instance()->GetGroupUsers(nId);
    }

    this->SltSendMessage(GetMyGroups, jsonArray);
}

/**
 * @brief ClientSocket::ParseRefreshFriend
 * @param dataVal
 */
void ClientSocket::ParseRefreshFriend(const QJsonValue &dataVal)
{
    QJsonArray jsonArray;
    // data 的 value 是数组
    if (dataVal.isArray()) {
        QJsonArray array = dataVal.toArray();
        int nSize = array.size();
        for (int i = 0; i < nSize; ++i) {
            int nId = array.at(i).toInt();
            jsonArray.append(DataBaseMagr::Instance()->GetUserStatus(nId));
        }
    }

    this->SltSendMessage(RefreshFriends, jsonArray);
}

/**
 * @brief ClientSocket::ParseRefreshGroups
 * 刷新当前群组里面的好友信息
 * @param dataVal
 */
void ClientSocket::ParseRefreshGroups(const QJsonValue &dataVal)
{
    QJsonArray jsonArray;
    // data 的 value 是数组
    if (dataVal.isObject()) {
        QJsonObject jsonObj = dataVal.toObject();
        // 群组ID
        int nId = jsonObj.value("id").toInt();
        jsonArray = DataBaseMagr::Instance()->GetGroupUsers(nId);
    }

    this->SltSendMessage(RefreshGroups, jsonArray);
}

/**
 * @brief ClientSocket::ParseMessages
 * 解析消息类，包括文字、图片、文件等
 * @param reply
 */
void ClientSocket::ParseFriendMessages(const QByteArray &reply)
{
    // 重新组装数据
    QJsonParseError jsonError;
    // 转化为 JSON 文档
    QJsonDocument doucment = QJsonDocument::fromJson(reply, &jsonError);
    // 解析未发生错误
    if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError)) {
        // JSON 文档为对象
        if (doucment.isObject()) {
            // 转化为对象
            QJsonObject jsonObj = doucment.object();
            int nType = jsonObj.value("type").toInt();
            QJsonValue dataVal = jsonObj.value("data");

            QJsonObject dataObj = dataVal.toObject();
            int nId = dataObj.value("to").toInt();
            Q_EMIT signalMsgToClient(nType, nId, dataObj);
        }
    }
}

/**
 * @brief ClientSocket::ParseGroupMessages
 * 处理群组消息转发
 * @param reply
 */
void ClientSocket::ParseGroupMessages(const QByteArray &reply)
{
    // 重新组装数据
    QJsonParseError jsonError;
    // 转化为 JSON 文档
    QJsonDocument doucment = QJsonDocument::fromJson(reply, &jsonError);
    // 解析未发生错误
    if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError)) {
        // JSON 文档为对象
        if (doucment.isObject()) {
            // 转化为对象
            QJsonObject jsonObj = doucment.object();
            int nType = jsonObj.value("type").toInt();
            QJsonValue dataVal = jsonObj.value("data");

            QJsonObject dataObj = dataVal.toObject();
            // 转发的群组id
            int nGroupId = dataObj.value("to").toInt();
            QString strMsg = dataObj.value("msg").toString();
            // 查询该群组下面的用户，一一转发消息
            QString name = DataBaseMagr::Instance()->GetUserName(m_nId);

            // 查询该群组里面的在线好友
            QJsonArray jsonArr = DataBaseMagr::Instance()->GetGroupUsers(nGroupId);
            if (jsonArr.size() < 2) return;
            for (int i = 1; i < jsonArr.size(); i++) {
                QJsonObject json = jsonArr.at(i).toObject();
                int nStatus = json.value("status").toInt();
                int nUserId = json.value("id").toInt();

                // 只给在线的好友转发消息
                if (OnLine == nStatus && m_nId != nUserId) {
                    // 重组消息
                    QJsonObject jsonMsg;
                    jsonMsg.insert("group", nGroupId);
                    jsonMsg.insert("id", m_nId);
                    jsonMsg.insert("name", name);
                    jsonMsg.insert("to", nUserId);
                    jsonMsg.insert("msg", strMsg);
                    jsonMsg.insert("head", DataBaseMagr::Instance()->GetUserHead(m_nId));

                    Q_EMIT signalMsgToClient(nType, nUserId, jsonMsg);
                }
            }
        }
    }
}

/**
 * @brief ClientSocket::SltSendMessage
 * @param type
 * @param json
 */
void ClientSocket::SltSendMessage(const quint8 &type, const QJsonValue &jsonVal)
{
    if (!m_tcpSocket->isOpen()) return;

    // 构建 Json 对象
    QJsonObject jsonObj;
    jsonObj.insert("type", type);
    jsonObj.insert("from", m_nId);
    jsonObj.insert("data", jsonVal);

    // 构建 Json 文档
    QJsonDocument document;
    document.setObject(jsonObj);
    m_tcpSocket->write(document.toJson(QJsonDocument::Compact));
}

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
ClientFileSocket::ClientFileSocket(QObject *parent, QTcpSocket *tcpSocket) :
    QObject(parent)
{
    // 将整个大的文件分成很多小的部分进行发送，每部分为4字节
    loadSize            = 50 * 1024;
    ullSendTotalBytes   = 0;
    ullRecvTotalBytes   = 0;
    bytesWritten        = 0;
    bytesToWrite        = 0;
    bytesReceived       = 0;

    fileNameSize        = 0;
    m_bBusy             = false;

    m_nUserId           = -1;
    m_nWindowId         = -1;

    // 本地文件存储
    fileToSend = new QFile(this);
    fileToRecv = new QFile(this);

    // 客户端
    if (tcpSocket == NULL) m_tcpSocket = new QTcpSocket(this);
    m_tcpSocket = tcpSocket;

    // 我们更新进度条
    connect(m_tcpSocket, SIGNAL(readyRead()), this, SLOT(SltReadyRead()));
    connect(m_tcpSocket, SIGNAL(disconnected()), this, SIGNAL(signalDisConnected()));
    // 当有数据发送成功时，我们更新进度条
    connect(m_tcpSocket, SIGNAL(bytesWritten(qint64)), this, SLOT(SltUpdateClientProgress(qint64)));
}

ClientFileSocket::~ClientFileSocket()
{

}

/**
 * @brief ClientFileSocket::Close
 * 关闭
 */
void ClientFileSocket::Close()
{
    m_tcpSocket->abort();
}

/**
 * @brief ClientFileSocket::CheckUserId
 * 用户socket检测，通过此函数进行判断连接的socket
 * @param nId
 * @param winId
 * @return
 */
bool ClientFileSocket::CheckUserId(const qint32 nId, const qint32 &winId)
{
    return ((m_nUserId == nId) && (m_nWindowId == winId));
}

/**
 * @brief ClientFileSocket::startTransferFile
 * 下发文件
 * @param type 0 表示单纯的语音文件，1表示文字+语音，客户端只收不显示
 * @param fileName  文件名
 */
void ClientFileSocket::StartTransferFile(QString fileName)
{
    if (m_bBusy) return;

    if (!m_tcpSocket->isOpen()) {
        return;
    }

    // 要发送的文件
    fileToSend = new QFile((-2 == m_nWindowId ? MyApp::m_strHeadPath : MyApp::m_strRecvPath) + fileName);

    if (!fileToSend->open(QFile::ReadOnly))
    {
        qDebug() << "open file error!";
        return;
    }

    ullSendTotalBytes = fileToSend->size(); // 文件总大小

    QDataStream sendOut(&outBlock, QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_4_8);

    QString currentFileName = fileName.right(fileName.size() - fileName.lastIndexOf('/')-1);

    // 依次写入总大小信息空间，文件名大小信息空间，文件名
    sendOut << qint64(0) << qint64(0) << currentFileName;

    // 这里的总大小是文件名大小等信息和实际文件大小的总和
    ullSendTotalBytes += outBlock.size();

    // 返回outBolock的开始，用实际的大小信息代替两个qint64(0)空间
    sendOut.device()->seek(0);
    sendOut << ullSendTotalBytes << qint64((outBlock.size() - sizeof(qint64)*2));

    // 发送完头数据后剩余数据的大小
    bytesToWrite = ullSendTotalBytes - m_tcpSocket->write(outBlock);

    outBlock.resize(0);
    m_bBusy = true;
    qDebug() << "Begin to send file" << fileName << m_nUserId << m_nWindowId;
}

/**
 * @brief ClientFileSocket::SltUpdateClientProgress
 * @param numBytes
 */
void ClientFileSocket::SltUpdateClientProgress(qint64 numBytes)
{
    // 已经发送数据的大小
    bytesWritten += (int)numBytes;
    // 如果已经发送了数据
    if (bytesToWrite > 0)
    {
        // 每次发送loadSize大小的数据，这里设置为4KB，如果剩余的数据不足4KB，就发送剩余数据的大小
        outBlock = fileToSend->read(qMin(bytesToWrite, loadSize));

        // 发送完一次数据后还剩余数据的大小
        bytesToWrite -= (int)m_tcpSocket->write(outBlock);

        // 清空发送缓冲区
        outBlock.resize(0);
    }
    else
    {
        // 如果没有发送任何数据，则关闭文件
        if (fileToSend->isOpen())
            fileToSend->close();
    }

    // 发送完毕
    if (bytesWritten >= ullSendTotalBytes)
    {
        if (fileToSend->isOpen())
            fileToSend->close();

        bytesWritten = 0;  // clear fot next send
        ullSendTotalBytes = 0;
        bytesToWrite = 0;
        qDebug() << "send ok" << fileToSend->fileName();
        FileTransFinished();
    }
}

void ClientFileSocket::displayError(QAbstractSocket::SocketError)
{
    m_tcpSocket->abort();
}

void ClientFileSocket::FileTransFinished()
{
    ullSendTotalBytes   = 0;
    ullRecvTotalBytes   = 0;
    bytesWritten        = 0;
    bytesToWrite        = 0;
    bytesReceived       = 0;

    fileNameSize        = 0;
    m_bBusy = false;
}

// 更新进度条，实现文件的接收
void ClientFileSocket::SltReadyRead()
{
    QDataStream in(m_tcpSocket);
    in.setVersion(QDataStream::Qt_4_8);

    // 连接时的消息
    if (0 == bytesReceived && (-1 == m_nUserId) && (-1 == m_nWindowId) &&
            (m_tcpSocket->bytesAvailable() == (sizeof(qint32) * 2)))
    {
        // 保存ID，方便发送文件
        in >> m_nUserId >> m_nWindowId;
        qDebug() << "File server Get userId" << m_nUserId << m_nWindowId;
        Q_EMIT signalConnected();
        return;
    }

    // 如果接收到的数据小于等于20个字节，那么是刚开始接收数据，我们保存为头文件信息
    if (bytesReceived <= (sizeof(qint64)*2))
    {
        int nlen = sizeof(qint64) * 2;
        // 接收数据总大小信息和文件名大小信息
        if ((m_tcpSocket->bytesAvailable() >= nlen) && (fileNameSize == 0))
        {
            in >> ullRecvTotalBytes >> fileNameSize;
            if (0 != ullRecvTotalBytes) bytesReceived += nlen;
        }

        // 接收文件名，并建立文件
        if((m_tcpSocket->bytesAvailable() >= (qint64)fileNameSize) &&
                ((qint64)fileNameSize != 0) &&
                (0 != ullRecvTotalBytes))
        {
            in >> fileReadName;
            bytesReceived += fileNameSize;

            fileToRecv->setFileName((-2 == m_nWindowId ? MyApp::m_strHeadPath : MyApp::m_strRecvPath) + fileReadName);

            if (!fileToRecv->open(QFile::WriteOnly | QIODevice::Truncate))
            {
                qDebug() << "open file error" << fileReadName;
                return;
            }
            qDebug() << "begin to recv files" << fileReadName;
        }
    }

    //如果接收的数据小于总数据，那么写入文件
    if (bytesReceived < ullRecvTotalBytes)
    {
        bytesReceived += m_tcpSocket->bytesAvailable();
        inBlock = m_tcpSocket->readAll();

        if (fileToRecv->isOpen())
            fileToRecv->write(inBlock);

        inBlock.resize(0);
    }

    // 接收数据完成时
    if ((bytesReceived >= ullRecvTotalBytes) && (0 != ullRecvTotalBytes))
    {
        fileToRecv->close();
        bytesReceived = 0; // clear for next receive
        ullRecvTotalBytes = 0;
        fileNameSize = 0;
        qDebug() << "recv ok" << fileToRecv->fileName();
        // 数据接受完成
        FileTransFinished();
    }
}
