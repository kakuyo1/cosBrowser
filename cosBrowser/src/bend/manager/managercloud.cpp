#include "managercloud.h"
#include "src/middle/managerglobal.h"
#include "src/middle/signals/managersignals.h"
#include "src/plugins/managerplugin.h"
#include <QDebug>
#include <src/bend/dao/cloud/daocloudsmock.h>
#include <QList>

Q_GLOBAL_STATIC(ManagerCloud, ins);

ManagerCloud::ManagerCloud(QObject* parent)  :
    QObject(parent)
{

}

ManagerCloud *ManagerCloud::instance()
{
    return ins();
}

void ManagerCloud::login(const QString &secretId, const QString &secretKey)
{
    QList<MyBucket> buckets = MANAGER_GLOBAL->managerPlugin->clouds()->login(secretId, secretKey);
    emit MANAGER_GLOBAL->managerSignals->loginSuccess(); // 登录成功
    bucketsAlready(buckets); // 成功获取存储桶列表
}

void ManagerCloud::getBuckets()
{
    QList<MyBucket> buckets = MANAGER_GLOBAL->managerPlugin->clouds()->buckets();
    bucketsAlready(buckets); // 成功获取存储桶列表
}

void ManagerCloud::putBucket(const QString &bucketName, const QString &location)
{
    MANAGER_GLOBAL->managerPlugin->clouds()->putBucket(bucketName, location);
    getBuckets(); // 重新获取存储桶列表(刷新)
}

void ManagerCloud::deleteBucket(const QString &bucketName)
{
    MANAGER_GLOBAL->managerPlugin->clouds()->deleteBucket(bucketName);
    emit MANAGER_GLOBAL->managerSignals->deleteBucketSuccess(bucketName); // 成功删除存储桶
    getBuckets(); // 重新获取存储桶列表(刷新)
}

void ManagerCloud::getObjects(const QString &bucketName, const QString &dir)
{
    qDebug() << "3.getObjects: " << bucketName << dir;
    m_currentBucketName = bucketName;
    m_currentDir = dir;
    QList<MyObject> objects = MANAGER_GLOBAL->managerPlugin->clouds()->getObjects(bucketName, dir);
    // 成功获取对象列表
    emit MANAGER_GLOBAL->managerSignals->objectsSuccess(objects);
}

void ManagerCloud::putObject(const QString& jobId,const QString& bucketName, const QString& objectKey,
                             const QString& localFilePath)
{
    // 定义进度回调函数
    auto callback = [=](qulonglong transferred_size, qulonglong total_size, void*){
        assert(transferred_size <= total_size);
        if (0 == transferred_size % (1024 * 512)) { // 每传输 512KB 回调一次(告诉前端更新进度条)
            emit MANAGER_GLOBAL->managerSignals->uploadProcess(jobId, transferred_size, total_size);
        }
    };
    // 调用上传接口
    MANAGER_GLOBAL->managerPlugin->clouds()->putObject(bucketName, objectKey, localFilePath, callback); // 阻塞了
    emit MANAGER_GLOBAL->managerSignals->uploadSuccess(jobId); // 上传完成
}

void ManagerCloud::getObject(const QString& jobId,const QString& bucketName, const QString& objectKey,
                             const QString& localFilePath)
{
    // 定义进度回调函数
    auto callback = [=](qulonglong transferred_size, qulonglong total_size, void*){
        assert(transferred_size <= total_size);
        if (0 == transferred_size % (1024 * 512)) { // 每传输 512KB 回调一次(告诉前端更新进度条)
            emit MANAGER_GLOBAL->managerSignals->downloadProcess(jobId, transferred_size, total_size);
        }
    };
    // 调用下载接口
    MANAGER_GLOBAL->managerPlugin->clouds()->getObject(bucketName, objectKey, localFilePath, callback); // 阻塞了
    emit MANAGER_GLOBAL->managerSignals->downloadSuccess(jobId); // 下载完成
}

QString ManagerCloud::currentBucketName() const
{
    return m_currentBucketName;
}

QString ManagerCloud::currentDir() const
{
    return m_currentDir;
}

void ManagerCloud::bucketsAlready(QList<MyBucket> buckets)
{
    m_currentBucketName.clear();
    m_currentDir.clear();
    emit MANAGER_GLOBAL->managerSignals->bucketsSuccess(buckets); // 成功获取存储桶列表
}
