#include "src/fend/uimain/uimain.h"
#include <QApplication>
#include <QMessageBox>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <src/middle/managerglobal.h>
#include <src/helper/filehelper.h>
#include <src/config/baseexception.h>
#include <src/config/global.h>
#include <src/bend/manager/managerdatabase.h>
#include <src/plugins/managerplugin.h>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    // 强制设置全局 locale
    setlocale(LC_ALL, "zh_CN.UTF-8");
    // 设置全局日志级别为debug
    spdlog::set_level(spdlog::level::debug);

    // 初始化所有单例
    try{
        MANAGER_GLOBAL->init(argc, argv);
    }catch(const QString& error)
    {
        // 捕获异常并显示 QMessageBox
        QMessageBox::critical(nullptr, "插件初始化错误", error);
        return -1; // 返回非零值表示错误
    }
    UiMain w;
    w.showLoginDialog();

    return a.exec();
}

// #include "cos_api.h"
// #include "cos_sys_config.h"
// #include "cos_defines.h"

// int main(int argc, char *argv[]) {
//     // 1. 指定配置文件路径，初始化 CosConfig
//     qcloud_cos::CosConfig config("./config.json");
//     qcloud_cos::CosAPI cos(config);

//     // 2. 构造创建存储桶的请求
//     std::string bucket_name = "examplebucket-1250000000";
//     // 替换为用户的存储桶名，由 bucketname-appid 组成，appid 必须填入，
//     // 可以在 COS 控制台查看存储桶名称。 https://console.cloud.tencent.com/cos5/bucket
//     qcloud_cos::PutBucketReq req(bucket_name);
//     qcloud_cos::PutBucketResp resp;

//     // 3. 调用创建存储桶接口
//     qcloud_cos::CosResult result = cos.PutBucket(req, &resp);

//     // 4. 处理调用结果
//     if (result.IsSucc()) {
//         // 创建成功
//     } else {
//         // 创建存储桶失败，可以调用 CosResult 的成员函数输出错误信息，例如 requestID 等
//         std::cout << "HttpStatus=" << result.GetHttpStatus() << std::endl;
//         std::cout << "ErrorCode=" << result.GetErrorCode() << std::endl;
//         std::cout << "ErrorMsg=" << result.GetErrorMsg() << std::endl;
//         std::cout << "ResourceAddr=" << result.GetResourceAddr() << std::endl;
//         std::cout << "XCosRequestId=" << result.GetXCosRequestId() << std::endl;
//         std::cout << "XCosTraceId=" << result.GetXCosTraceId() << std::endl;
//     }
// }
