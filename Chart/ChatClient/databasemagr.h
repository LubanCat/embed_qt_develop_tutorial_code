#ifndef DATABASEMAGR_H
#define DATABASEMAGR_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMutex>

class ItemInfo;

/////////////////////////////////////////////////////////////
/// \brief The DataBaseMagr class
/// 客户端数据库管理类
class DataBaseMagr : public QObject
{
    Q_OBJECT
public:
    explicit DataBaseMagr(QObject *parent = 0);
    ~DataBaseMagr();

    bool OpenUserDb(const QString &dataName);
    bool OpenMessageDb(const QString &dataName);

    // 单实例运行
    static DataBaseMagr *Instance()
    {
        static QMutex mutex;
        if (NULL == self) {
            QMutexLocker locker(&mutex);

            if (!self) {
                self = new DataBaseMagr();
            }
        }

        return self;
    }

    // 添加历史聊天记录
    void AddHistoryMsg(const int &userId, ItemInfo *itemInfo);

    // 添加好友，保存至数据库
    void AddFriend(const int &id, const int &userId, const QString &name);
    // 添加群组
    void AddGroup(const int &id, const int &userId, const QString &name);

    // 删除好友
    bool DeleteMyFriend(const int &id, const int &userId);

    // 获取我的好友
    QJsonArray GetMyFriend(const int &userId) const;
    // 获取我的群组
    QJsonArray GetMyGroup(const int &userId) const;

    // 判断改好友是否已经是我的好友了
    bool isMyFriend(const int &userId, const QString &name);
    // 判断是否已经加入该群组了
    bool isInGroup(const QString &name);

    // 获取历史聊天记录
    QVector<ItemInfo *> QueryHistory(const int &id, const int &count = 0);

    // 测试使用，打印数据库中的所有信息
    void QueryAll();
signals:

public slots:

private:
    static DataBaseMagr *self;

    // 数据库管理
    QSqlDatabase userdb;
    QSqlDatabase msgdb;
};

#endif // DATABASEMAGR_H
