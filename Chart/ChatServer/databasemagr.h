#ifndef DATABASEMAGR_H
#define DATABASEMAGR_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMutex>

/////////////////////////////////////////////////////////////////
/// \brief The DataBaseMagr class
/// 服务端数据库管理
class DataBaseMagr : public QObject
{
    Q_OBJECT
public:
    explicit DataBaseMagr(QObject *parent = 0);
    ~DataBaseMagr();

    bool OpenDb(const QString &dataName);
    void CloseDb();

    // 单实例
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

    // 更新用户状态/上下线
    void UpdateUserStatus(const int &id, const quint8 &status);
    void UpdateUserHead(const int &id, const QString &strHead);

    void TestHeadPic(const int &id, const QString &name, const QString &strHead);

    // 获取用户状态
    QJsonArray GetAllUsers();
    QJsonObject GetUserStatus(const int &id) const;
    int GetUserLineStatus(const int &id) const;

    // 用户校验
    QJsonObject CheckUserLogin(const QString &name, const QString &passwd);
    // 注册用户
    int RegisterUser(const QString &name, const QString &passwd);

    // 添加好友
    QJsonObject AddFriend(const QString &name);

    // 添加群组
    QJsonObject AddGroup(const int &userId, const QString &name);
    // 创建群组
    QJsonObject CreateGroup(const int &userId, const QString &name);
    // 查询当前群组下面的好友
    QJsonArray  GetGroupUsers(const int &groupId);

    // 服务器启动的时候更新下所以人员的状态，可以不要
    void ChangeAllUserStatus();

    // 查询当前id的名字
    QString GetUserName(const int &id) const;
    QString GetUserHead(const int &id) const;
    QJsonObject GetUserInfo(const int &id) const;
signals:

public slots:

private:
    static DataBaseMagr *self;

    QSqlDatabase userdb;

    void QueryAll();
};

#endif // DATABASEMAGR_H
