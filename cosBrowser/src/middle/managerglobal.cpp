#include "managerglobal.h"
#include "managermodels.h"
#include <src/bend/gateway.h>
#include <src/middle/signals/managersignals.h>
#include <src/plugins/managerplugin.h>
#include <src/bend/manager/managercloud.h>
#include <src/bend/manager/managerdatabase.h>
#include <QApplication>
#include <src/helper/filehelper.h>
#include "src/config/global.h"
Q_GLOBAL_STATIC(ManagerGlobal, ins)

ManagerGlobal::ManagerGlobal(QObject *parent)
    : QObject{parent},
      gateway(new GateWay(this)),
      managerSignals(new ManagerSignals(this)),
      managerPlugin(new ManagerPlugin(this)),
      managerDataBase(new ManagerDataBase(this)),
      managerCloud(new ManagerCloud(this))
{

}

ManagerGlobal::~ManagerGlobal()
{
    delete gateway;
    qDebug() << "ManagerGlobal::~ManagerGlobal() called";
    delete managerSignals;
    qDebug() << "ManagerGlobal::~ManagerGlobal() after deleting managerSignals";
    delete managerPlugin;
    qDebug() << "ManagerGlobal::~ManagerGlobal() after deleting managerPlugin";
    delete managerDataBase;
    qDebug() << "ManagerGlobal::~ManagerGlobal() after deleting managerDataBase";
    delete managerCloud;
    qDebug() << "ManagerGlobal::~ManagerGlobal() after deleting managerCloud";
}

ManagerGlobal *ManagerGlobal::instance()
{
    return ins();
}

void ManagerGlobal::init(int argc, char *argv[])
{
    // 初始化model单例
    managerModels = new ManagerModels(this);

    // 创建必要的文件夹TMP放置临时文件如数据库文件
    FileHelper::mkPath(CONFIG::PATH::TMP);

    managerPlugin->installPlugins(argc, argv);

    QString qssStr = FileHelper::readAllText(":/static/qss/default.qss");
    qApp->setStyleSheet(qssStr); // 设置全局样式表

    managerDataBase->init();
}

