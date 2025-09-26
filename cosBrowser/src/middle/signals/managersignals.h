#ifndef MANAGERSIGNALS_H
#define MANAGERSIGNALS_H

#include <QObject>

#include <src/middle/models/mybucket.h>
#include <QList>

class ManagerSignals : public QObject
{
    Q_OBJECT
public:
    explicit ManagerSignals(QObject *parent = nullptr);
    ~ManagerSignals();
signals:
    // 登录成功
    void loginSuccess();

    void unLogin();

    // 报错(接口 + 信息 + 请求信息)
    void error(int api, const QString& message, const QJsonValue& requestInfo);

    // 返回用户对应存储桶列表
    void bucketsSuccess(const QList<MyBucket>& buckets);

    // 成功获取对象列表
    void objectsSuccess(const QList<MyObject>& objects);

    // 成功删除存储桶
    void deleteBucketSuccess(const QString& bucketName);

    // 开始上传/下载信号
    void startUpload(const QString& jobId, const QString& key, const QString& localPath);
    void startDownload(const QString &jobId, const QString &key, const QString &localPath, qulonglong totalSize);

    // 上传/下载进度提示信号
    void uploadProcess(const QString& jobId, qulonglong transferred_size, qulonglong total_size);
    void downloadProcess(const QString& jobId, qulonglong transferred_size, qulonglong total_size);
    //上传/下载成功信号
    void uploadSuccess(const QString& jobId);
    void downloadSuccess(const QString& jobId);
};

#endif // MANAGERSIGNALS_H
