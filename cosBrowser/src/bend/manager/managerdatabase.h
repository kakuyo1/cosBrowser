#ifndef MANAGERDATABASE_H
#define MANAGERDATABASE_H

#include <QObject>

#include <src/bend/dao/db/daologininfo.h>
#include <src/bend/dao/db/daologininfosqlite.h>

/*
业务逻辑层(Business Logic Layer)： ManagerDatabase
                                                                        V
数据访问层 (Data Access Layer - DAL)： DaoLoginInfo (Data Access Object)---->暂时只有登录信息的增删改查
                                                                        V
数据源层 (Data Source Layer)：           dbSqlite---> QSqlDatabase (由Qt框架提供)
*/



#define MANAGER_DATABASE ManagerDataBase::instance()

class ManagerDataBase : public QObject
{
    Q_OBJECT
public:
    explicit ManagerDataBase(QObject *parent = nullptr);
    static ManagerDataBase* instance();
    void init();

    void saveOrUpdateLoginInfo(const QString& name, const QString& secretId, const QString& secretKey, const QString& remark);
    void removeLoginInfo(const QString& secretId);

    int indexOfLoginInfo(const QString& secretId) const;

    QStringList loginNameList();
    LoginInfo loginInfoByName(const QString& name);
signals:

private:
    DaoLoginInfoSqlite daoLoginInfo;
    QList<LoginInfo> loginInfoList; // 缓存登录信息列表, 单个用户可能有多个secretId, 或者说多个账号
};

#endif // MANAGERDATABASE_H
