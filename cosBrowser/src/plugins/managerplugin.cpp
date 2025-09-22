#include "managerplugin.h"

#include <src/bend/dao/cloud/daocloudscos.h>
#include <src/bend/dao/cloud/daocloudsmock.h>
#include <src/bend/dao/configs/versioncmd.h>
#include <src/bend/dao/configs/versionjson.h>
#include "src/config/global.h"
#include <spdlog/spdlog.h>
Q_GLOBAL_STATIC(ManagerPlugin, ins);

ManagerPlugin::ManagerPlugin(QObject *parent)
    : QObject{parent}
{
}

ManagerPlugin::~ManagerPlugin()
{
    delete m_clouds;
    delete m_version;
}

ManagerPlugin *ManagerPlugin::instance()
{
    return ins();
}

void ManagerPlugin::installPlugins(int argc, char* argv[])
{
    // 优先级 ： 命令行参数 > 配置文件
    VersionCmd version(argc, argv);
    if (version.isVersionValid()) {
        m_version = new VersionCmd(argc, argv);
    } else {
        m_version = new VersionJson(CONFIG::VERSION::JSON_PATH);
    }

    m_version->setVersion();
    if (m_version->majorVersion() == CONFIG::VERSION::MAJOR_BUSINESS) {
        m_clouds = new DaoCloudsCos(); // 业务版本使用真实的云服务接口
        spdlog::info("Tencent Cloud DaoCloudsCos loaded");
    } else {
        m_clouds = new DaoCloudsMock(":/static/testing/custom.json"); // 定制版使用mock数据
        spdlog::info("Mock DaoCloudsMock loaded");
    }
}

DaoClouds *ManagerPlugin::clouds() const
{
    return m_clouds;
}
