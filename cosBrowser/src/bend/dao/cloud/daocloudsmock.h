#ifndef DAOCLOUDSMOCK_H
#define DAOCLOUDSMOCK_H

#include "daoclouds.h"

#include <QJsonValue>

class DaoCloudsMock : public DaoClouds
{
public:
    explicit DaoCloudsMock(const QString& path);
    virtual ~DaoCloudsMock();
    virtual QList<MyBucket> buckets() override;

    virtual QList<MyBucket> login(const QString& secretId, const QString& secretKey) override;

    virtual bool isBucketExists(const QString& bucketName) override;
    virtual QString getBucketLocation(const QString& bucketName) override;
    virtual MyBucket getBucketByName(const QString& bucketName) override;
    virtual void putBucket(const QString& bucketName, const QString& location) override;
    virtual void deleteBucket(const QString& bucketName) override;

    virtual QList<MyObject> getObjects(const QString& bucketName, const QString& dir) override;
    virtual bool isObjectExists(const QString& bucketName, const QString& objectKey) override;
    // 上传文件
    virtual void putObject(const QString& bucketName, const QString& objectKey,
                           const QString& localFilePath, const TransProgressCallBack& callback) override;
    // 下载文件
    virtual void getObject(const QString& bucketName, const QString& objectKey,
                           const QString& localPath, const TransProgressCallBack& callback) override;
private:
    QJsonValue m_mock;
};

#endif // DAOCLOUDSMOCK_H
