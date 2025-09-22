#include "dbmysql.h"
#include <QSqlError>

DBMysql::DBMysql() {
    m_db = QSqlDatabase::addDatabase("MYSQL");
}

DBMysql::~DBMysql()
{
    if (m_db.isOpen()) {
        m_db.close();
    }
}

void DBMysql::connect(const QString &dbPath)
{
    m_db.setDatabaseName(dbPath);
    if (!m_db.open()) {
        throw std::runtime_error("无法连接到数据库: " + m_db.lastError().text().toStdString());
    }
}
