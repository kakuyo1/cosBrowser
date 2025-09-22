#include "daocloudsmock.h"
#include "qjsonarray.h"
#include "src/config/errorcode.h"
#include <src/helper/filehelper.h>

#include <QJsonObject>
#include <QThread>

#include <src/config/baseexception.h>

DaoCloudsMock::DaoCloudsMock(const QString& path) {
    m_mock = FileHelper::readAllJson(path).toJsonValue();
}

DaoCloudsMock::~DaoCloudsMock()
{

}

QList<MyBucket> DaoCloudsMock::buckets()
{
    QList<MyBucket> list;
    QJsonArray array = m_mock["buckets"].toArray();
    for (const auto& item : std::as_const(array)) {
        MyBucket bucket;
        QJsonObject obj = item.toObject();
        bucket.name = obj["name"].toString();
        bucket.location = obj["location"].toString();
        bucket.createDate = obj["create_date"].toString();
        list.append(bucket);
        qDebug() << bucket.name << bucket.location << bucket.createDate;
    }
    return list;
}

QList<MyBucket> DaoCloudsMock::login(const QString &secretId, const QString &secretKey)
{
    QJsonArray array = m_mock["users"].toArray();
    for (const auto& item : std::as_const(array)) {
        QJsonObject obj = item.toObject();
        if (obj["secretId"].toString() == secretId
            && obj["secretKey"].toString() == secretKey) {
            return buckets();// 因为是mock数据，这里没有对应的存储桶，所有用户用的同一个存储桶
        }
    }
    throw BaseException(EC_211000, "Wrong SecretId or SecretKey"); // 抛出异常一直传输到gateway的send函数处进行处理
}

bool DaoCloudsMock::isBucketExists(const QString &bucketName)
{
    Q_UNUSED(bucketName);
    return false;
}

QString DaoCloudsMock::getBucketLocation(const QString &bucketName)
{
    Q_UNUSED(bucketName);
    return "";
}

MyBucket DaoCloudsMock::getBucketByName(const QString &bucketName)
{
    Q_UNUSED(bucketName);
    return MyBucket();
}

void DaoCloudsMock::putBucket(const QString &bucketName, const QString &location)
{
    Q_UNUSED(bucketName);
    Q_UNUSED(location);
}

void DaoCloudsMock::deleteBucket(const QString &bucketName)
{
    Q_UNUSED(bucketName);
}

QList<MyObject> DaoCloudsMock::getObjects(const QString &bucketName, const QString &dir)
{
    Q_UNUSED(bucketName);
    Q_UNUSED(dir);
    return QList<MyObject>();
}

bool DaoCloudsMock::isObjectExists(const QString &bucketName, const QString &objectKey)
{
    Q_UNUSED(bucketName);
    Q_UNUSED(objectKey);
    return false;
}

void DaoCloudsMock::putObject(const QString &bucketName, const QString &objectKey, const QString &localFilePath, const TransProgressCallBack &callback)
{
    Q_UNUSED(bucketName);
    Q_UNUSED(objectKey);
    Q_UNUSED(localFilePath);
    Q_UNUSED(callback);
}

void DaoCloudsMock::getObject(const QString &bucketName, const QString &objectKey, const QString &localPath, const TransProgressCallBack &callback)
{
    Q_UNUSED(bucketName);
    Q_UNUSED(objectKey);
    Q_UNUSED(localPath);
    Q_UNUSED(callback);
}
