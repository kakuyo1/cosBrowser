#ifndef DBMYSQL_H
#define DBMYSQL_H

#include <QSqlDatabase>
#include <QString>

class DBMysql
{
public:
    DBMysql();
    ~DBMysql();

    void connect(const QString& dbPath);

private:
    QSqlDatabase m_db;
};

#endif // DBMYSQL_H
