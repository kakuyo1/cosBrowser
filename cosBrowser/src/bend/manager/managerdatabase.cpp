#include "managerdatabase.h"
#include <QDateTime>

Q_GLOBAL_STATIC(ManagerDataBase, ins)

ManagerDataBase::ManagerDataBase(QObject *parent)
    : QObject{parent}
{}

ManagerDataBase *ManagerDataBase::instance()
{
    return ins();
}


void ManagerDataBase::init()
{
    daoLoginInfo.connect();
    daoLoginInfo.createTable();
    loginInfoList = daoLoginInfo.select();
}

void ManagerDataBase::saveOrUpdateLoginInfo(const QString &name, const QString &secretId, const QString &secretKey, const QString &remark)
{
    LoginInfo info;
    info.name = (name == "" ? secretId : name);
    info.secret_id = secretId;
    info.secret_key = secretKey;
    info.remark = remark;
    info.timestamp = QDateTime::currentSecsSinceEpoch();// UTC时间戳,比如 1696118400
    if (daoLoginInfo.exists(secretId)) {
        daoLoginInfo.update(info);
        loginInfoList.replace(indexOfLoginInfo(secretId), info); // 更新缓存
    } else {
        daoLoginInfo.insert(info);
        loginInfoList.append(info); // 更新缓存
    }
}

void ManagerDataBase::removeLoginInfo(const QString &secretId)
{
    if (daoLoginInfo.exists(secretId)) {
        daoLoginInfo.remove(secretId);
        loginInfoList.removeAt(indexOfLoginInfo(secretId)); // 更新缓存)
    }
}

int ManagerDataBase::indexOfLoginInfo(const QString &secretId) const
{
    for (int i = 0; i < loginInfoList.size(); ++i) {
        if (loginInfoList.at(i).secret_id == secretId) {
            return i;
        }
    }
    throw std::runtime_error(QString("没有找到对应的secretId: %1").arg(secretId).toStdString());
}

QStringList ManagerDataBase::loginNameList()
{
    QStringList names;
    for (const LoginInfo& info : std::as_const(loginInfoList)) {
        if (!names.contains(info.name)) {
            names.append(info.name);
        }
    }
    return names;
}

LoginInfo ManagerDataBase::loginInfoByName(const QString &name)
{
    for (const LoginInfo& info : std::as_const(loginInfoList)) {
        if (info.name == name) {
            return info;
        }
    }
    throw std::runtime_error(QString("没有找到对应的登录名: %1").arg(name).toStdString());
}
