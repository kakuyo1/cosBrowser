#ifndef MANAGERCLOUD_H
#define MANAGERCLOUD_H

#include <QObject>
#include <QStandardItemModel>


#define MANAGER_CLOUD ManagerCloud::instance()
struct MyBucket;
class ManagerCloud : public QObject
{
    Q_OBJECT
public:
    explicit ManagerCloud(QObject *parent = nullptr);

    static ManagerCloud* instance();

    void login(const QString& secretId, const QString& secretKey);

    void getBuckets();

    void putBucket(const QString& bucketName, const QString& location);

    void deleteBucket(const QString& bucketName);

    void getObjects(const QString& bucketName, const QString& dir);

    void putObject(const QString& jobId,const QString& bucketName, const QString& objectKey,
                   const QString& localFilePath);

    void getObject(const QString& jobId,const QString& bucketName, const QString& objectKey,
                   const QString& localPath);

    QString currentBucketName() const;
    QString currentDir() const;
    void bucketsAlready(QList<MyBucket> buckets);
private:
    QString m_currentBucketName;
    QString m_currentDir;
};

#endif // MANAGERCLOUD_H
