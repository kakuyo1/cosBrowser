#include "daologininfosqlite.h"
#include "src/config/global.h"

DaoLoginInfoSqlite::DaoLoginInfoSqlite() {}

bool DaoLoginInfoSqlite::exists(const QString &secretId)
{
    QSqlQuery query;
    query.prepare(QString("SELECT id FROM %1 WHERE secret_id = :secretId")
                      .arg(CONFIG::TABLES::LOGIN_INFO));
    query.bindValue(":secretId", secretId);
    return m_db_proxy.exists(query);
}

void DaoLoginInfoSqlite::insert(const LoginInfo &info)
{
    QSqlQuery query;
    QString sql = QString(
                      "INSERT INTO %1 (name, secret_id, secret_key, remark, timestamp) "
                      "VALUES (?, ?, ?, ?, ?);")
                      .arg(CONFIG::TABLES::LOGIN_INFO);

    if (!query.prepare(sql)) {
        qWarning() << "DaoLoginInfo::insert prepare failed:" << query.lastError().text();
        throw std::runtime_error(("prepare failed: " + query.lastError().text()).toStdString());
    }

    query.addBindValue(info.name);
    query.addBindValue(info.secret_id);
    query.addBindValue(info.secret_key);
    query.addBindValue(info.remark);
    query.addBindValue(info.timestamp);

    if (!query.exec()) {
        qWarning() << "DaoLoginInfo::insert exec failed:" << query.lastError().text() << "SQL:" << sql;
        throw std::runtime_error(("exec failed: " + query.lastError().text()).toStdString());
    }
}

void DaoLoginInfoSqlite::update(const LoginInfo &info)
{
    QSqlQuery query;
    QString sql = QString(
                      "UPDATE %1 SET name = ?, secret_key = ?, remark = ?, timestamp = ? "
                      "WHERE secret_id = ?;")
                      .arg(CONFIG::TABLES::LOGIN_INFO);

    if (!query.prepare(sql)) {
        qWarning() << "DaoLoginInfo::update prepare failed:" << query.lastError().text();
        throw std::runtime_error(("prepare failed: " + query.lastError().text()).toStdString());
    }

    query.addBindValue(info.name);
    query.addBindValue(info.secret_key);
    query.addBindValue(info.remark);
    query.addBindValue(info.timestamp);
    query.addBindValue(info.secret_id);

    if (!query.exec()) {
        qWarning() << "DaoLoginInfo::update exec failed:" << query.lastError().text() << "SQL:" << sql;
        throw std::runtime_error(("exec failed: " + query.lastError().text()).toStdString());
    }
}

void DaoLoginInfoSqlite::remove(const QString &secretId)
{
    QSqlQuery query;
    QString sql = QString("DELETE FROM %1 WHERE secret_id = ?;")
                      .arg(CONFIG::TABLES::LOGIN_INFO);

    if (!query.prepare(sql)) {
        qWarning() << "DaoLoginInfo::remove prepare failed:" << query.lastError().text();
        throw std::runtime_error(("prepare failed: " + query.lastError().text()).toStdString());
    }

    query.addBindValue(secretId);

    if (!query.exec()) {
        qWarning() << "DaoLoginInfo::remove exec failed:" << query.lastError().text() << "SQL:" << sql;
        throw std::runtime_error(("exec failed: " + query.lastError().text()).toStdString());
    }
}

QList<LoginInfo> DaoLoginInfoSqlite::select()
{
    // 按时间降序排列，最新登录的信息显示在最上面
    QString sql = QString("SELECT name, secret_id, secret_key, remark, timestamp FROM %1 "
                          "order by timestamp desc;")
                      .arg(CONFIG::TABLES::LOGIN_INFO);

    QList<LoginInfo> list;
    QList<RECORD> records = m_db_proxy.select(sql);
    for (const RECORD& record : std::as_const(records)) {
        LoginInfo info;
        info.name = record.value("name").toString();
        info.secret_id = record.value("secret_id").toString();
        info.secret_key = record.value("secret_key").toString();
        info.remark = record.value("remark").toString();
        info.timestamp = record.value("timestamp").toUInt();
        list.append(info);
    }
    return list;
}

void DaoLoginInfoSqlite::connect()
{
    m_db_proxy.connect(CONFIG::SQLITE::NAME);
}

void DaoLoginInfoSqlite::createTable()
{
    QString sql = FileHelper::readAllText(CONFIG::SQL::LOGIN_INFO_TABLE);
    m_db_proxy.exec(sql);
}
