#include "dbsqlite.h"
#include <QSqlRecord>

DBSqlite::DBSqlite() {
    m_db = QSqlDatabase::addDatabase("QSQLITE");
}

DBSqlite::~DBSqlite()
{
    if (m_db.isOpen()) {
        m_db.close();
    }
}

void DBSqlite::connect(const QString &dbPath)
{
    m_db.setDatabaseName(dbPath);
    if (!m_db.open()) {
        throw std::runtime_error("无法连接到数据库: " + m_db.lastError().text().toStdString());
    }
}

QSqlQuery DBSqlite::exec(const QString &sql)
{
    QSqlQuery query;
    if (!query.exec(sql)) {
        throw std::runtime_error("执行SQL失败: " + query.lastError().text().toStdString());
    }
    return query;
}

bool DBSqlite::exists(QSqlQuery &query)
{
    if (!query.exec()) {
        qWarning() << "SQL exists query failed:" << query.lastError().text();
        return false;
    }
    return query.next(); // 如果有结果，说明存在
}

QList<QMap<QString, QVariant> > DBSqlite::select(const QString& sql)
{
    QList<RECORD> records;
    QSqlQuery query = exec(sql);
    while (query.next()) {
        RECORD record; // 每一行记录
        for (int i = 0; i < query.record().count(); ++i) {
            QString fieldName = query.record().fieldName(i);
            QVariant value = query.value(i);
            record.insert(fieldName, value);
        }
        records.append(record);
    }
    return records;
}
