#include "objectswidget.h"
#include "src/middle/managermodels.h"
#include "ui_objectswidget.h"
#include "src/middle/managerglobal.h"
#include <src/fend/uidelegates/bucketdelegate.h>

ObjectsWidget::ObjectsWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ObjectsWidget)
{
    ui->setupUi(this);
    ui->tableViewObjects->setModel(MANAGER_GLOBAL->managerModels->modelBuckets());
    ui->tableViewObjects->setItemDelegateForColumn(1, new BucketDelegate()); // 为第二列设置下拉框
    // 设置标题内容
    QStandardItemModel* model = MANAGER_GLOBAL->managerModels->modelBuckets();
    QStringList headers = {"存储桶名称", "存储桶地域", "创建时间"};
    model->setColumnCount(headers.size());
    model->setHorizontalHeaderLabels(headers);
    // 调整列宽自适应内容
    ui->tableViewObjects->setColumnWidth(0, 200);
    ui->tableViewObjects->setColumnWidth(1, 120);
    ui->tableViewObjects->horizontalHeader()->setStretchLastSection(true); // 最后一列拉伸填满
    // 隐藏垂直表头
    ui->tableViewObjects->verticalHeader()->setVisible(false);
    // 设置鼠标点击排序
    ui->tableViewObjects->setSortingEnabled(true);
}

ObjectsWidget::~ObjectsWidget()
{
    delete ui;
}
