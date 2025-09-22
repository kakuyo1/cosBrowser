#ifndef DAOCLOUDSCOS_H
#define DAOCLOUDSCOS_H
#include "daoclouds.h"

#include <cos_config.h>

#include <response/bucket_resp.h>
class DaoCloudsCos : public DaoClouds
{
public:
    DaoCloudsCos();
    virtual ~DaoCloudsCos();
    virtual QList<MyBucket> buckets() override;

    virtual QList<MyBucket> login(const QString& secretId, const QString& secretKey) override;

    virtual bool isBucketExists(const QString& bucketName) override;
    virtual QString getBucketLocation(const QString& bucketName) override;

    virtual void putBucket(const QString& bucketName, const QString& location) override;
    virtual void deleteBucket(const QString& bucketName) override;

    virtual MyBucket getBucketByName(const QString& bucketName) override;
    virtual QList<MyObject> getObjects(const QString& bucketName, const QString& dir) override;
    virtual bool isObjectExists(const QString& bucketName, const QString& objectKey) override;
    // 上传文件
    virtual void putObject(const QString& bucketName, const QString& objectKey,
                           const QString& localFilePath, const TransProgressCallBack& callback) override;
    // 下载文件
    virtual void getObject(const QString& bucketName, const QString& objectKey,
                           const QString& localPath, const TransProgressCallBack& callback) override;
private:
    QList<MyObject> getDirList(qcloud_cos::GetBucketResp& resp, const QString& dir);
    QList<MyObject> getFileList(qcloud_cos::GetBucketResp& resp, const QString& dir);
private:
    qcloud_cos::CosConfig *m_config = nullptr;

};

#endif // DAOCLOUDSCOS_H
