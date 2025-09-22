#ifndef DAOLOGININFOSQLITE_H
#define DAOLOGININFOSQLITE_H

#include "daologininfo.h"

#include <src/helper/dbsqlite.h>

class DaoLoginInfoSqlite : public DaoLoginInfo
{
public:
    DaoLoginInfoSqlite();

    virtual bool exists(const QString& secretId) override;
    virtual void insert(const LoginInfo& info) override;
    virtual void update(const LoginInfo& info) override;
    virtual void remove(const QString& secretId) override;
    virtual QList<LoginInfo> select() override;
    virtual void connect() override;
    virtual void createTable() override;
private:
    DBSqlite m_db_proxy;
};

#endif // DAOLOGININFOSQLITE_H
