#include "bucketsListwidget.h"
#include "src/bend/gateway.h"
#include "src/config/apis.h"
#include "src/middle/managerglobal.h"
#include "src/middle/managermodels.h"
#include "src/middle/signals/managersignals.h"
#include "ui_bucketsListwidget.h"

#include <QJsonObject>

BucketsWidget::BucketsWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::BucketsWidget)
{
    ui->setupUi(this);
    ui->listViewBuckets->setModel(MANAGER_GLOBAL->managerModels->modelBuckets());
    // 设置样式
    ui->listViewBuckets->setStyleSheet(R"(
    QListView {
        border: none;
    }
    QListView::item {
        height: 30px;
    }
    QListView::item:hover,
    QListView::item:selected {
        background-color: rgba(221, 221, 221, 0.4); /* 等价于 #66dddddd */
        color: blue;
    })");
    // 信号槽连接
    connect(MANAGER_GLOBAL->managerSignals, &ManagerSignals::bucketsSuccess, this, &BucketsWidget::onBucketsSuccess);
    connect(ui->lineEditSearch, &ComboLine::itemSelected, this, &BucketsWidget::showBucketObjects);
    connect(ui->lineEditSearch, &ComboLine::textChanged, this, &BucketsWidget::onTextChanged);
}

BucketsWidget::~BucketsWidget()
{
    delete ui;
}

void BucketsWidget::on_listViewBuckets_doubleClicked(const QModelIndex &index)
{
    showBucketObjects(index.data().toString());
}

void BucketsWidget::onBucketsSuccess(const QList<MyBucket> &buckets)
{
    QStringList bucketItems;
    for (const MyBucket& bucket : buckets) {
        bucketItems.append(bucket.name);
    }
    ui->lineEditSearch->setItems(bucketItems);
}

void BucketsWidget::showBucketObjects(const QString &bucketName)
{
    QJsonObject params;
    params["bucketName"] = bucketName;
    params["dir"] = ""; // 根目录
    qDebug() << "1.双击存储桶:";
    MANAGER_GLOBAL->gateway->send(API::OBJECTS::LIST, params);
}

void BucketsWidget::onTextChanged(const QString &text)
{
    QStandardItemModel* model = MANAGER_GLOBAL->managerModels->modelBuckets();
    for (int i = 0; i < model->rowCount(); ++i) {
        // isShow 代表当前行是否显示
        bool isShow = model->index(i, 0).data().toString().contains(text, Qt::CaseInsensitive);
        ui->listViewBuckets->setRowHidden(i, !isShow);
    }
}

