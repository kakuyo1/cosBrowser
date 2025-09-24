#include "bucketstablewidget.h"
#include "src/bend/gateway.h"
#include "src/config/apis.h"
#include "src/middle/managerglobal.h"
#include "src/middle/managermodels.h"
#include "createbucketdialog.h"
#include "ui_bucketstablewidget.h"
#include "src/config/apis.h"
#include <src/fend/uidelegates/bucketdelegate.h>

#include <QJSONObject>
#include <QJsonObject>
#include <QMessageBox>

#include <src/middle/signals/managersignals.h>

BucketsTableWidget::BucketsTableWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::BucketsTableWidget)
{
    ui->setupUi(this);
    ui->tableView->setModel(MANAGER_GLOBAL->managerModels->modelBuckets());
    ui->tableView->setItemDelegateForColumn(1, new BucketDelegate()); // 为第二列设置下拉框

    // 调整列宽自适应内容
    ui->tableView->setColumnWidth(0, 200);
    ui->tableView->setColumnWidth(1, 120);
    ui->tableView->horizontalHeader()->setStretchLastSection(true); // 最后一列拉伸填满
    // 隐藏垂直表头
    ui->tableView->verticalHeader()->setVisible(false);
    // 设置鼠标点击排序
    ui->tableView->setSortingEnabled(true);

    // 初始化翻页按钮
    ui->widgetPageFlip->setRowPerPageItems({2, 5, 10, 20});

    // 添加右键弹出删除桶菜单
    ui->tableView->setContextMenuPolicy(Qt::ActionsContextMenu); // ActionsContextMenu 允许通过右键点击显示动作菜单
    QAction* deleteAction = new QAction("删除存储桶", this);
    ui->tableView->addAction(deleteAction);

    // 信号槽连接
    connect(ui->widgetPageFlip, &PageFlipWidget::pageNumberChanged,this, &BucketsTableWidget::onPageNumberChanged);
    connect(MANAGER_GLOBAL->managerSignals, &ManagerSignals::bucketsSuccess, this, &BucketsTableWidget::onBucketsSuccess);
    connect(deleteAction, &QAction::triggered, this, &BucketsTableWidget::onDeleteActionTriggered);
}

BucketsTableWidget::~BucketsTableWidget()
{
    delete ui;
}

void BucketsTableWidget::on_tableView_doubleClicked(const QModelIndex &index)
{
    if(index.column() == 0) { // 仅当双击第一列时响应
        QJsonObject params;
        params["bucketName"] = index.data().toString();
        params["dir"] = ""; // 根目录
        MANAGER_GLOBAL->gateway->send(API::OBJECTS::LIST, params);
    }
}

void BucketsTableWidget::onPageNumberChanged(int startIndexInModel, int rowsToShow)
{
    // 筛选出需要显示的行，其他行隐藏（这样就可以显示出一页的内容）
    QStandardItemModel* model = MANAGER_GLOBAL->managerModels->modelBuckets();
    for (int i = 0; i < model->rowCount(); ++i) {
        // isShow 代表当前行是否显示
        bool isShow = (i >= startIndexInModel && i < startIndexInModel + rowsToShow);
        ui->tableView->setRowHidden(i, !isShow);
    }
}

void BucketsTableWidget::onBucketsSuccess(const QList<MyBucket> &buckets)
{
    // 每次获取到新的存储桶列表时，设置翻页控件总行数
    qDebug() << "BucketsTableWidget::onBucketsSuccess: " << buckets.size();
    ui->widgetPageFlip->setTotalRow(buckets.size());
}


void BucketsTableWidget::on_pushButtonCreateBucket_clicked()
{
    CreateBucketDialog dialog(this);
    int rec = dialog.exec();
    if (rec == QDialog::Accepted) {
        MyBucket bucket = dialog.getBucket();
        if (bucket.isValid()) {
            QJsonObject params;
            params["bucketName"] = bucket.name;
            params["location"] = bucket.location;
            MANAGER_GLOBAL->gateway->send(API::BUCKETS::PUT, params);
        }
    }
}

void BucketsTableWidget::onDeleteActionTriggered()
{
    QModelIndex index = ui->tableView->currentIndex();
    QString bucketName = index.data().toString();
    if (index.isValid()) { // 仅当选中行时响应
        QMessageBox box(QMessageBox::Question, "删除存储桶", QString("确定要删除该存储桶[%1]吗？").arg(bucketName),
            QMessageBox::NoButton, this);
        // 添加按钮并指定角色
        QPushButton *yesButton = box.addButton("确定", QMessageBox::YesRole);
        QPushButton *noButton  = box.addButton("取消", QMessageBox::NoRole);
        box.exec();
        if (box.clickedButton() == yesButton) {
            QJsonObject params;
            params["bucketName"] = bucketName;
            MANAGER_GLOBAL->gateway->send(API::BUCKETS::DEL, params);
        } else if (box.clickedButton() == noButton) {
            return; // 用户选择不删除
        }
    }
}


void BucketsTableWidget::on_pushButtonRefresh_clicked()
{
    MANAGER_GLOBAL->gateway->send(API::BUCKETS::LIST, QJsonObject());
}

