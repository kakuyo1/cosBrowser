#include "gateway.h"
#include "src/bend/manager/managercloud.h"
#include <QtConcurrent>
#include <spdlog/spdlog.h>
#include "src/config/apis.h"
#include "src/config/errorcode.h"
#include "src/middle/managerglobal.h"
#include "src/config/baseexception.h"
#include "src/middle/signals/managersignals.h"
Q_GLOBAL_STATIC(GateWay, ins)

GateWay::GateWay(QObject *parent)
    : QObject{parent}
{}

GateWay *GateWay::instance()
{
    return ins();
}

//这两种方式是等价的：
//QtConcurrent::run([](){ /* task */ });
//等同于
//QThreadPool::globalInstance()->start([](){ /* task */ });

void GateWay::send(int api, const QJsonValue &value)
{
    // Qt 框架提供的一个全局线程池的单例访问接口。它返回一个应用程序范围内共享的 QThreadPool 实例。
    // 实际上，QtConcurrent::run() 在底层就是使用全局线程池
    QThreadPool::globalInstance()->start([=](){
        try {
            this->dispatch(api,value);
        }
        catch (const BaseException &e) {
            spdlog::warn("Error in GateWay::send: {}", e.message().toLocal8Bit().constData());
            emit MANAGER_GLOBAL->managerSignals->error(api, e.message(), QJsonValue());
        }
        catch(const std::exception &e) {
            spdlog::warn("std::exception in GateWay::send: {}", e.what());
            BaseException be(EC_100000, QString("std::exception: %1").arg(e.what()));
            emit MANAGER_GLOBAL->managerSignals->error(api, be.message(), QJsonValue());
        }
        catch(...) {
            BaseException e = BaseException(EC_100000, "未知错误");
            spdlog::warn("Unknown error in GateWay::send");
            emit MANAGER_GLOBAL->managerSignals->error(api, e.message(), QJsonValue());
        }
    });
}

void GateWay::dispatch(int api, const QJsonValue &value)
{
    // 路由表
    switch (api) {
    case API::LOGIN::NORMAL:
        apiLogin(value);
        break;
    case API::BUCKETS::LIST:
        apiBucketsList(value);
        break;
    case API::BUCKETS::PUT:
        apiBucketsPut(value);
        break;
    case API::BUCKETS::DEL:
        apiBucketsDelete(value);
        break;
    case API::OBJECTS::LIST:
        apiObjectsList(value);
        break;
    case API::OBJECTS::GET:
        apiObjectsGet(value);
        break;
    case API::OBJECTS::PUT:
        apiObjectsPut(value);
        break;
    default:
        break;
    }
}

void GateWay::apiLogin(const QJsonValue &value)
{
    QString secretId = value["secretId"].toString();
    QString secretKey = value["secretKey"].toString();
    MANAGER_GLOBAL->managerCloud->login(secretId, secretKey);
}

void GateWay::apiBucketsList(const QJsonValue &value)
{
    Q_UNUSED(value);
    MANAGER_GLOBAL->managerCloud->getBuckets();
}

void GateWay::apiBucketsPut(const QJsonValue &value)
{
    // 数据由前端传入
    QString bucketName = value["bucketName"].toString();
    QString location = value["location"].toString();
    MANAGER_GLOBAL->managerCloud->putBucket(bucketName, location);
    spdlog::info("New Bucket Created: {} : {}", bucketName.toStdString(), location.toStdString());
}

void GateWay::apiBucketsDelete(const QJsonValue &value)
{
    QString bucketName = value["bucketName"].toString();
    MANAGER_GLOBAL->managerCloud->deleteBucket(bucketName);
    spdlog::info("Bucket Deleted: {}", bucketName.toStdString());
}

void GateWay::apiObjectsList(const QJsonValue &value)
{
    qDebug() << "2.apiObjectsList";
    QString bucketName = value["bucketName"].toString();
    QString dir = value["dir"].toString();
    MANAGER_GLOBAL->managerCloud->getObjects(bucketName, dir);
}

void GateWay::apiObjectsGet(const QJsonValue &value)
{
    QString jobId = value["jobId"].toString(); // uuid
    QString bucketName = value["bucketName"].toString(); // 桶名称
    QString objectKey = value["objectKey"].toString(); // 文件在云端的完整路径
    QString localPath = value["localPath"].toString(); // 下载到本地的路径
    MANAGER_GLOBAL->managerCloud->getObject(jobId, bucketName, objectKey, localPath);
    spdlog::info("Object Downloaded: {} : {} -> {}", bucketName.toStdString(), objectKey.toStdString(), localPath.toStdString());
}

void GateWay::apiObjectsPut(const QJsonValue &value)
{
    QString jobId = value["jobId"].toString(); // uuid
    QString bucketName = value["bucketName"].toString(); // 桶名称
    QString objectKey = value["objectKey"].toString(); // 文件在云端的完整路径
    QString localFilePath = value["localFilePath"].toString(); // 上传的本地文件路径
    MANAGER_GLOBAL->managerCloud->putObject(jobId, bucketName, objectKey, localFilePath);
    spdlog::info("Object Uploaded: {} : {} -> {}", bucketName.toStdString(), localFilePath.toStdString(), objectKey.toStdString());
}
