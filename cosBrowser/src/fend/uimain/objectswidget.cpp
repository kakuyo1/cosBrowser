#include "objectswidget.h"
#include "src/bend/gateway.h"
#include "src/bend/manager/managercloud.h"
#include "src/middle/managermodels.h"
#include "src/middle/signals/managersignals.h"
#include "ui_objectswidget.h"
#include "src/middle/managerglobal.h"
#include <src/fend/uidelegates/bucketdelegate.h>
#include "src/config/apis.h"
#include <QJsonObject>

ObjectsWidget::ObjectsWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ObjectsWidget)
{
    ui->setupUi(this);
    ui->tableViewObjects->setModel(MANAGER_GLOBAL->managerModels->modelObjects());

    // 调整列宽自适应内容
    ui->tableViewObjects->setColumnWidth(0, 200);
    ui->tableViewObjects->setColumnWidth(1, 120);
    ui->tableViewObjects->horizontalHeader()->setStretchLastSection(true); // 最后一列拉伸填满
    // 隐藏垂直表头
    ui->tableViewObjects->verticalHeader()->setVisible(false);
    // 设置鼠标点击排序以及view代理
    ui->tableViewObjects->setSortingEnabled(true);

    // 初始化翻页按钮
    ui->widgetPageFlip->setRowPerPageItems({2, 5, 10, 20});

    // 信号槽连接
    connect(MANAGER_GLOBAL->managerSignals, &ManagerSignals::objectsSuccess, this, &ObjectsWidget::onObjectsSuccess);
    connect(ui->widgetBreadCrumb, &BreadCrumdWidget::pathChanged, this, &ObjectsWidget::onPathChanged);
    connect(ui->widgetPageFlip, &PageFlipWidget::pageNumberChanged,this, &ObjectsWidget::onPageNumberChanged);
}

ObjectsWidget::~ObjectsWidget()
{
    delete ui;
}

void ObjectsWidget::on_pushButtonBucketList_clicked()
{
    MANAGER_GLOBAL->gateway->send(API::BUCKETS::LIST, QJsonObject{});
}


void ObjectsWidget::on_tableViewObjects_doubleClicked(const QModelIndex &index) // 下钻功能
{
    MyObject object = index.data(Qt::UserRole).value<MyObject>();
    if (object.isDir()) { // 仅当双击的是目录时响应（下钻）
        QJsonObject params;
        params["bucketName"] = MANAGER_GLOBAL->managerCloud->currentBucketName();
        params["dir"] = object.key; // 注意这里填的是key(对象完整路径), 不是dir
        qDebug() << "object:key: " << object.key;
        MANAGER_GLOBAL->gateway->send(API::OBJECTS::LIST, params);
    }
}

void ObjectsWidget::onObjectsSuccess(const QList<MyObject> &objects)
{
    QString path = MANAGER_GLOBAL->managerCloud->currentBucketName() + "/"
            + MANAGER_GLOBAL->managerCloud->currentDir(); // 当前路径, 如 bucket/dir1/dir2
    ui->widgetBreadCrumb->setPath(path);
    ui->widgetPageFlip->setTotalRow(objects.size());
}

void ObjectsWidget::onPathChanged(const QString &newPath) // 上钻功能
{
    // newPath = file-1300416378/test/bll
    // 转换为key：test/bll/
    QString key = newPath + "/";
    key = key.mid(key.indexOf('/') + 1); // 去掉前缀部分，获取key
    QJsonObject params;
    params["bucketName"] = MANAGER_GLOBAL->managerCloud->currentBucketName();
    params["dir"] = key;
    MANAGER_GLOBAL->gateway->send(API::OBJECTS::LIST, params);
}

void ObjectsWidget::onPageNumberChanged(int startIndexInModel, int rowsToShow)
{
    // 筛选出需要显示的行，其他行隐藏（这样就可以显示出一页的内容）
    QStandardItemModel* model = MANAGER_GLOBAL->managerModels->modelObjects();
    for (int i = 0; i < model->rowCount(); ++i) {
        // isShow 代表当前行是否显示
        bool isShow = (i >= startIndexInModel && i < startIndexInModel + rowsToShow);
        ui->tableViewObjects->setRowHidden(i, !isShow);
    }
}


void ObjectsWidget::on_pushButtonRefresh_clicked()
{
    // 刷新当前路径
    onPathChanged(ui->widgetBreadCrumb->currentPath());
}

