#include "uiupload.h"
#include "src/middle/managerglobal.h"
#include "ui_uiupload.h"
#include <src/config/apis.h>
#include <src/middle/signals/managersignals.h>
#include <spdlog/spdlog.h>
#include <QFileInfo>
#include <QJsonObject>
#include <src/fend/uidelegates/tableitemdelegate.h>

UiUpload::UiUpload(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UiUpload)
{
    ui->setupUi(this);
    initHeader();
    connect(MANAGER_GLOBAL->managerSignals, &ManagerSignals::startUpload, this, &UiUpload::onStartUpload);
    connect(MANAGER_GLOBAL->managerSignals, &ManagerSignals::uploadProcess, this, &UiUpload::onUploadProgress);
    connect(MANAGER_GLOBAL->managerSignals, &ManagerSignals::uploadSuccess, this, &UiUpload::onUploadSuccess);
    connect(MANAGER_GLOBAL->managerSignals, &ManagerSignals::error, this, &UiUpload::onError);

    // 设置样式
    ui->tableWidget->setStyleSheet(R"(
    QTableWidget {
        border: none;
        border-top: 1px solid #dddddd;
    }
    QTableWidget::item {
        border-bottom: 1px solid #dddddd;
    }
    QTableWidget::item:hover,
    QTableWidget::item:selected {
        background-color: rgba(221, 221, 221, 0.4); /* 等价于 #66dddddd */
        color: blue;
    })");

    // 设置itemdelegate
    ui->tableWidget->setItemDelegate(new TableItemDelegate(ui->tableWidget));
}

UiUpload::~UiUpload()
{
    delete ui;
}

void UiUpload::onStartUpload(const QString &jobId, const QString &key, const QString &localPath)
{
    // 在第一行插入新行
    ui->tableWidget->insertRow(0);
    ui->tableWidget->setRowHeight(0, 40);
    // 1.云端路径
    QTableWidgetItem* itemCloudPath = new QTableWidgetItem(key);
    itemCloudPath->setData(Qt::UserRole, jobId); // 将 jobId 存储在 UserRole 中，方便后续查找
    ui->tableWidget->setItem(0, 0, itemCloudPath);
    // 2.本地路径
    QTableWidgetItem* itemLocalPath = new QTableWidgetItem(localPath);
    ui->tableWidget->setItem(0, 1, itemLocalPath);
    // 3.上传状态(进度条)
    QFileInfo fileInfo(localPath);
    UiProgressBarWidget* progressBarWidget = new UiProgressBarWidget();
    progressBarWidget->setRange(0, fileInfo.size()); // 设置进度条范围为文件大小
    ui->tableWidget->setCellWidget(0, 2, progressBarWidget); // 将进度条控件添加到表格中
}

void UiUpload::onUploadProgress(const QString &jobId, qulonglong transferred_size, qulonglong total_size)
{
    Q_UNUSED(total_size);
    // 根据 jobId 查找对应的进度条控件, 然后更新进度
    UiProgressBarWidget* progressBarWidget = findTableWidgetByJobId(jobId);
    if (progressBarWidget) {
        progressBarWidget->setValue(transferred_size);
    } else {
        spdlog::warn("UiUpload::onUploadProgress: 未找到对应的进度条控件, jobId: {}", jobId.toStdString());
    }
}

void UiUpload::onUploadSuccess(const QString &jobId)
{
    UiProgressBarWidget* progressBarWidget = findTableWidgetByJobId(jobId);
    if (progressBarWidget) {
        progressBarWidget->setFinished("上传完成");
    } else {
        spdlog::warn("UiUpload::onUploadSuccess: 未找到对应的进度条控件, jobId: {}", jobId.toStdString());
    }
}

void UiUpload::onError(int api, const QString &message, const QJsonValue &requestInfo)
{
    if (api != API::OBJECTS::PUT) {
        return; // 仅处理上传错误
    }
    QString jobId = requestInfo.toObject().value("jobId").toString();
    UiProgressBarWidget* progressBarWidget = findTableWidgetByJobId(jobId);
    if (progressBarWidget) {
        progressBarWidget->setError("上传失败: " + message);
        progressBarWidget->setToolTip(message); // 设置鼠标悬停时显示的提示信息
    } else {
        spdlog::warn("UiUpload::onError: 未找到对应的进度条控件, jobId: {}", jobId.toStdString());
    }
}

UiProgressBarWidget *UiUpload::findTableWidgetByJobId(const QString &jobId)
{
    // 遍历表格的每一行，查找对应 jobId 的进度条控件
    for (int i = 0; i < ui->tableWidget->rowCount(); ++i) {
        QTableWidgetItem* item = ui->tableWidget->item(i, 0); // 获取第一列的项(里面用Qt::UserRole存了jobId)
        QString itemJobId = item->data(Qt::UserRole).toString();
        if (itemJobId == jobId) {
            return qobject_cast<UiProgressBarWidget*>(ui->tableWidget->cellWidget(i, 2)); // 返回第三列的进度条控件
        }
    }
    return nullptr; // 未找到
}

void UiUpload::initHeader()
{
    QStringList headers = {"云端路径", "本地路径", "上传状态"};
    ui->tableWidget->setColumnCount(headers.size());
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true); // 最后一列拉伸填满
}
