#include "daocloudscos.h"
#include "qdebug.h"
#include "src/config/baseexception.h"
#include "src/config/errorcode.h"
#include "cos_api.h"
#include <spdlog/spdlog.h>

/*
    变量及API介绍：
1. qcloud_cos::CosConfig: 用于配置COS客户端的参数，如AccessKey、SecretKey、区域等。（有点像endpoint）
2. qcloud_cos::CosAPI: COS客户端的主要接口类，用于执行各种COS操作：（有点像socket）
   - GetService: 获取用户的存储桶列表。
   - PutBucket: 创建存储桶。
   - DeleteBucket: 删除存储桶。
   - ListObjects: 列出存储桶中的对象。
   - PutObject: 上传对象。
   - GetObject: 下载对象。
   - DeleteObject: 删除对象。
3. qcloud_cos::CosResult: 用于表示COS操作的结果，包含是否成功、错误码、错误信息等。
4. qcloud_cos::GetServiceReq / GetServiceResp: 分别表示获取存储桶列表的请求和响应。
5. qcloud_cos::Bucket: 表示存储桶的信息，包括名称、区域、创建时间等。
6. qcloud_cos::Object: 表示存储桶中的对象信息，包括对象键、大小、最后修改时间等。
7. qcloud_cos::PutObjectReq / PutObjectResp: 分别表示上传对象的请求和响应。
8. qcloud_cos::GetObjectReq / GetObjectResp: 分别表示下载对象的请求和响应。
9. qcloud_cos::DeleteObjectReq / DeleteObjectResp: 分别表示删除对象的请求和响应。
10. qcloud_cos::ListObjectsReq / ListObjectsResp: 分别表示列出对象的请求和响应。

每次调用 cos.GetService(req, &resp)，就相当于：
1.用一个 socket 打开连接
2.发送 “GetService” 请求包
3.接收响应数据
4.然后关闭/复用连接
5.所以它是一个 操作句柄，背后帮你管了 HTTP 请求/应答。
*/

DaoCloudsCos::DaoCloudsCos() {
    // 这里需要放到执行文件(.exe)所在目录的上一个目录
    m_config = new qcloud_cos::CosConfig("B:/qtlearing/cosMain9-3/build/Desktop_Qt_6_9_0_MSVC2022_64bit-Release/release/cosconfig.json");
    spdlog::info("CosConfig loaded from ./cosconfig.json");
}

DaoCloudsCos::~DaoCloudsCos()
{
    if (m_config) {
        delete m_config;
        m_config = nullptr;
    }
}

QList<MyBucket> DaoCloudsCos::buckets()
{
    QList<MyBucket> res;

    qcloud_cos::GetServiceReq req;
    qcloud_cos::GetServiceResp resp;
    qcloud_cos::CosAPI cos(*m_config); // 每次调用都创建一个新的CosAPI对象，确保线程安全
    qcloud_cos::CosResult result = cos.GetService(req, &resp); // result包含请求结果，resp包含响应数据
    if (result.IsSucc()) {
        std::vector<qcloud_cos::Bucket> buckets = resp.GetBuckets(); // resp包含响应数据
        for (std::vector<qcloud_cos::Bucket>::const_iterator it = buckets.begin(); it != buckets.end(); ++it) {
            const qcloud_cos::Bucket& bucket = *it; // 获取每个存储桶的信息
            MyBucket myBucket;
            myBucket.name = QString::fromStdString(bucket.m_name.c_str());
            myBucket.location = QString::fromStdString(bucket.m_location.c_str());
            myBucket.createDate = QString::fromStdString(bucket.m_create_date.c_str());
            res.append(myBucket);
        }
    } else {
        QString errorMessage = QString("腾讯云 [错误码]: %1, [错误信息]: %2").arg(result.GetErrorCode().c_str(), result.GetErrorMsg().c_str());
        throw BaseException(EC_211000, errorMessage);
    }
    return res;
}

QList<MyBucket> DaoCloudsCos::login(const QString &secretId, const QString &secretKey)
{
    // 交给腾讯云自动完成鉴权和登陆
    m_config->SetAccessKey(secretId.toStdString()); // AccessKey (SecretId)
    m_config->SetSecretKey(secretKey.toStdString());
    return buckets();
}

bool DaoCloudsCos::isBucketExists(const QString &bucketName)
{
    // 实时获取最新的存储桶列表（不需要关心location，直接获取全部并进行比较，防止location写错造成错误）
    QList<MyBucket> list = buckets();
    for (const MyBucket& bucket : std::as_const(list)) {
        if (bucket.name == bucketName) {
            return true;
        }
    }
    return false;
}

QString DaoCloudsCos::getBucketLocation(const QString &bucketName)
{
    // 1. 优先通过API获取存储桶的地域信息
    qcloud_cos::CosAPI cos(*m_config);
    QString location = QString::fromStdString(cos.GetBucketLocation(bucketName.toStdString()));
    if (location != "") {
        return location;
    }
    // 2. 如果API获取失败，则从本地存储桶列表中查找
    MyBucket bucket = getBucketByName(bucketName);
    if (bucket.name != "") {
        return bucket.location;
    }
    throw BaseException(EC_332000, QString("存储桶 %1 不存在").arg(bucketName));
}

void DaoCloudsCos::putBucket(const QString &bucketName, const QString &location)
{
    if (isBucketExists(bucketName)) {
        throw BaseException(EC_212000, QString("存储桶 %1 已经存在").arg(bucketName));
    }
    m_config->SetRegion(location.toStdString());
    qcloud_cos::PutBucketReq req(bucketName.toStdString());
    qcloud_cos::PutBucketResp resp;
    qcloud_cos::CosAPI cos(*m_config);
    qcloud_cos::CosResult result = cos.PutBucket(req, &resp);
    if (!result.IsSucc()) {
        QString errorMessage = QString("腾讯云 [错误码]: %1, [错误信息]: %2").arg(result.GetErrorCode().c_str(), result.GetErrorMsg().c_str());
        throw BaseException(EC_331100, errorMessage);
    }
}

void DaoCloudsCos::deleteBucket(const QString &bucketName)
{
    if (!isBucketExists(bucketName)) {
        throw BaseException(EC_212000, QString("存储桶 %1 不存在").arg(bucketName));
    }
    QString location = getBucketLocation(bucketName);
    m_config->SetRegion(location.toStdString());
    qcloud_cos::DeleteBucketReq req(bucketName.toStdString());
    qcloud_cos::DeleteBucketResp resp;
    qcloud_cos::CosAPI cos(*m_config);
    qcloud_cos::CosResult result = cos.DeleteBucket(req, &resp);
    if (!result.IsSucc()) {
        QString errorMessage = QString("腾讯云 [错误码]: %1, [错误信息]: %2").arg(result.GetErrorCode().c_str(), result.GetErrorMsg().c_str());
        throw BaseException(EC_331300, errorMessage);
    }
}

MyBucket DaoCloudsCos::getBucketByName(const QString &bucketName)
{
    QList<MyBucket> bs = buckets();
    for (const auto& b: qAsConst(bs)) {
        if (b.name == bucketName) {
            return b;
        }
    }
    return MyBucket();
}

/*
1.Prefix：筛选出以这个字符串开头的所有路径
2.Delimiter：在这些路径中，找到第一个分隔符出现的位置，然后按分隔符前的部分进行分组
简单比喻：
1.Prefix 相当于 cd 进入某个目录
2.Delimiter 相当于 ls 时看到的是子目录列表，而不是所有深层文件
*/

QList<MyObject> DaoCloudsCos::getObjects(const QString &bucketName, const QString &dir)
{
    qDebug() << "4.DaoCloudsCos::getObjects: " << bucketName << dir;
    qcloud_cos::GetBucketReq req(bucketName.toStdString());
    qcloud_cos::GetBucketResp resp;
    if (dir != "") { // 说明是获取某个指定目录下的内容
        req.SetPrefix(dir.toStdString()); // 设置前缀，表示只列出以该前缀开头的对象
    }
    req.SetDelimiter("/"); // 找到第一个分隔符出现的位置，然后按分隔符前的部分进行分组

    QString location = getBucketLocation(bucketName);
    m_config->SetRegion(location.toStdString());
    qcloud_cos::CosAPI cos(*m_config);
    qcloud_cos::CosResult result = cos.GetBucket(req, &resp); // 获取存储桶中的对象列表
    if (!result.IsSucc()) {
        QString errorMessage = QString("腾讯云 [错误码]: %1, [错误信息]: %2").arg(result.GetErrorCode().c_str(), result.GetErrorMsg().c_str());
        throw BaseException(EC_331200, errorMessage);
    }
    return getDirList(resp, dir) + getFileList(resp, dir); // 合并目录和文件列表
}

bool DaoCloudsCos::isObjectExists(const QString &bucketName, const QString &objectKey)
{
    QString location = getBucketLocation(bucketName);
    m_config->SetRegion(location.toStdString());
    qcloud_cos::CosAPI cos(*m_config);
    return cos.IsObjectExist(bucketName.toStdString(), objectKey.toStdString());
}

/*
API及变量说明：
1.SharedAsyncContext: 用于异步操作的上下文，包含回调函数等信息。
2.AsyncPutObjectReq: 异步上传对象的请求。
3.AsyncGetObjectReq: 异步下载对象的请求。
*/

void DaoCloudsCos::putObject(const QString &bucketName, const QString &objectKey, const QString &localFilePath, const TransProgressCallBack &callback)
{
    // 初始化上下文
    qcloud_cos::SharedAsyncContext context;
    // 异步上传
    qcloud_cos::AsyncPutObjectReq req(bucketName.toStdString(), objectKey.toStdString(), localFilePath.toStdString());
    // 设置进度回调函数
    if (callback) {
        req.SetTransferProgressCallback(callback);
    }
    // 获取地域
    QString location = getBucketLocation(bucketName);
    m_config->SetRegion(location.toStdString());
    qcloud_cos::CosAPI cos(*m_config);

    // 开始上传
    context = cos.AsyncPutObject(req);
    // 等待上传完成
    context->WaitUntilFinish();// 阻塞等待上传完成（保持 WaitUntilFinish，但是把整个 putObject 封装到一个后台线程执行，这样 UI 不会被卡。）
    qcloud_cos::CosResult result = context->GetResult();
    if (!result.IsSucc()) {
        QString errorMessage = QString("腾讯云 [错误码]: %1, [错误信息]: %2").arg(result.GetErrorCode().c_str(), result.GetErrorMsg().c_str());
        throw BaseException(EC_332400, errorMessage);
    }
}

void DaoCloudsCos::getObject(const QString &bucketName, const QString &objectKey, const QString &localPath, const TransProgressCallBack &callback)
{
    // 初始化上下文
    qcloud_cos::SharedAsyncContext context;
    // 异步下载
    qcloud_cos::AsyncGetObjectReq req(bucketName.toStdString(), objectKey.toStdString(), localPath.toStdString());
    // 设置进度回调函数
    if (callback) {
        req.SetTransferProgressCallback(callback);
    }
    // 获取地域
    QString location = getBucketLocation(bucketName);
    m_config->SetRegion(location.toStdString());
    qcloud_cos::CosAPI cos(*m_config);

    // 开始下载
    context = cos.AsyncGetObject(req);
    // 等待下载完成
    context->WaitUntilFinish();
    qcloud_cos::CosResult result = context->GetResult();
    if (!result.IsSucc()) {
        QString errorMessage = QString("腾讯云 [错误码]: %1, [错误信息]: %2").arg(result.GetErrorCode().c_str(), result.GetErrorMsg().c_str());
        throw BaseException(EC_332500, errorMessage);
    }
}

QList<MyObject> DaoCloudsCos::getDirList(qcloud_cos::GetBucketResp &resp, const QString &dir)
{
    QList<MyObject> list;
    std::vector<std::string> prefixes = resp.GetCommonPrefixes(); // 获取公共前缀列表，即目录列表
    for (int i =0; i < prefixes.size(); ++i) {
        QString prefix = QString::fromStdString(prefixes[i]);
        MyObject obj;
        obj.dir = dir;
        obj.name = prefix.mid(dir.size()); // 去掉前缀部分，获取目录名
        obj.size = 0;
        obj.lastModified = "-"; // 目录没有最后修改时间
        obj.key = prefix;
        list.append(obj);
        qDebug() << "5.Dir: " << obj.dir << obj.name;
    }
    return list;
}

QList<MyObject> DaoCloudsCos::getFileList(qcloud_cos::GetBucketResp &resp, const QString &dir)
{
    QList<MyObject> list;
    std::vector<qcloud_cos::Content> files = resp.GetContents(); // 获取对象列表，即文件列表
    for (int i =0; i < files.size(); ++i) {
        const qcloud_cos::Content& file = files[i];
        QString file_key(file.m_key.c_str()); // 文件的完整路径
        QString file_name = file_key.mid(dir.size());// 去掉前缀部分，获取文件名
        if (file_key != dir) { // 过滤掉如test/的文件夹本身
            MyObject obj;
            obj.dir = dir;
            obj.name = file_name;
            obj.size = QString(file.m_size.c_str()).toULongLong();
            obj.lastModified = QString::fromStdString(file.m_last_modified);
            obj.key = file_key;
            list.append(obj);
            qDebug() << "6.File: " << obj.dir << obj.name << obj.size << obj.lastModified;
        }
    }
    return list;
}
