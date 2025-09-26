#include "uidownload.h"
#include "src/middle/managerglobal.h"
#include "src/middle/signals/managersignals.h"
#include "ui_uidownload.h"
#include "src/config/apis.h"
#include <QFileInfo>
#include <QJsonObject>
#include <spdlog/spdlog.h>
#include <src/fend/uidelegates/tableitemdelegate.h>

UiDownload::UiDownload(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UiDownload)
{
    ui->setupUi(this);
    initHeader();
    connect(MANAGER_GLOBAL->managerSignals, &ManagerSignals::startDownload, this, &UiDownload::onStartDownload);
    connect(MANAGER_GLOBAL->managerSignals, &ManagerSignals::downloadProcess, this, &UiDownload::onDownloadProgress);
    connect(MANAGER_GLOBAL->managerSignals, &ManagerSignals::downloadSuccess, this, &UiDownload::onDownloadSuccess);
    connect(MANAGER_GLOBAL->managerSignals, &ManagerSignals::error, this, &UiDownload::onError);

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

UiDownload::~UiDownload()
{
    delete ui;
}

void UiDownload::onStartDownload(const QString &jobId, const QString &key, const QString &localPath, qulonglong totalSize)
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
    UiProgressBarWidget* progressBarWidget = new UiProgressBarWidget();
    progressBarWidget->setRange(0, totalSize); // 设置进度条范围为文件大小
    ui->tableWidget->setCellWidget(0, 2, progressBarWidget); // 将进度条控件添加到表格中
}

void UiDownload::onDownloadProgress(const QString &jobId, qulonglong transferred_size, qulonglong total_size)
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

void UiDownload::onDownloadSuccess(const QString &jobId)
{
    UiProgressBarWidget* progressBarWidget = findTableWidgetByJobId(jobId);
    if (progressBarWidget) {
        progressBarWidget->setFinished("下载完成");
    } else {
        spdlog::warn("UiUpload::onUploadSuccess: 未找到对应的进度条控件, jobId: {}", jobId.toStdString());
    }
}

void UiDownload::onError(int api, const QString &message, const QJsonValue &requestInfo)
{
    if (api != API::OBJECTS::GET) {
        return; // 仅处理下载错误
    }
    QString jobId = requestInfo.toObject().value("jobId").toString();
    UiProgressBarWidget* progressBarWidget = findTableWidgetByJobId(jobId);
    if (progressBarWidget) {
        progressBarWidget->setError("下载失败: " + message);
        progressBarWidget->setToolTip(message); // 设置鼠标悬停时显示的提示信息
    } else {
        spdlog::warn("UiUpload::onError: 未找到对应的进度条控件, jobId: {}", jobId.toStdString());
    }
}

UiProgressBarWidget *UiDownload::findTableWidgetByJobId(const QString &jobId)
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

void UiDownload::initHeader()
{
    QStringList headers({"云端路径", "本地路径", "下载状态"});
    ui->tableWidget->setColumnCount(headers.size());
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
}
