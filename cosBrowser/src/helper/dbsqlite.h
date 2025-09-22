#ifndef DBSQLITE_H
#define DBSQLITE_H
#include <QString>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>
#include <QMap>
#include <QList>

using RECORD = QMap<QString, QVariant>;

class DBSqlite
{
public:
    DBSqlite();
    ~DBSqlite();
    void connect(const QString& dbPath);
    QSqlQuery exec(const QString& sql);
    bool exists(QSqlQuery &query);
    QList<RECORD> select(const QString& sql);
private:
    QSqlDatabase m_db;
};

#endif // DBSQLITE_H
