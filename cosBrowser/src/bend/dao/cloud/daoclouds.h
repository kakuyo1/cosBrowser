#ifndef DAOCLOUDS_H
#define DAOCLOUDS_H

#include <src/middle/models/mybucket.h>

#include <functional>

// 传输进度回调函数类型定义
// 每次上传有进展时，SDK 会调用你传进去的回调函数，并且把当前已经传输的字节数、总字节数传给你。
// 在 COS SDK 的实现里，上传大文件时会自动分片（比如每片 1MB 或 4MB），SDK 内部是按块发的。
// 腾讯云 COS C++ SDK 在 上传文件时，会不断触发你设置的 TransferProgressCallback，
// 每触发一次代表 SDK 已经确认有一部分字节成功写到网络流里。
// 换句话说，每次回调时，可以认为有一个数据块成功传输了，但你拿到的是“累计值”。
using TransProgressCallBack = std::function<void (uint64_t transferred_size, uint64_t total_size, void*)>;

class DaoClouds
{
public:
    DaoClouds() {};
    virtual ~DaoClouds() {};
    virtual QList<MyBucket> buckets() = 0;
    virtual QList<MyBucket> login(const QString& secretId, const QString& secretKey) = 0;

    virtual bool isBucketExists(const QString& bucketName) = 0;
    virtual QString getBucketLocation(const QString& bucketName) = 0;

    virtual void putBucket(const QString& bucketName, const QString& location) = 0;
    virtual void deleteBucket(const QString& bucketName) = 0;

    virtual MyBucket getBucketByName(const QString& bucketName) = 0;
    // 获取指定目录下的对象列表
    virtual QList<MyObject> getObjects(const QString& bucketName, const QString& dir) = 0;
    virtual bool isObjectExists(const QString& bucketName, const QString& objectKey) = 0;
    // 上传文件
    virtual void putObject(const QString& bucketName, const QString& objectKey,
                           const QString& localFilePath, const TransProgressCallBack& callback) = 0;
    // 下载文件
    virtual void getObject(const QString& bucketName, const QString& objectKey,
                           const QString& localPath, const TransProgressCallBack& callback) = 0;
};

#endif // DAOCLOUDS_H
