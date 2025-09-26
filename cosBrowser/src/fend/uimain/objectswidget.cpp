#include "objectswidget.h"
#include "src/bend/gateway.h"
#include "src/bend/manager/managercloud.h"
#include "src/middle/managermodels.h"
#include "src/middle/signals/managersignals.h"
#include "ui_objectswidget.h"
#include "src/middle/managerglobal.h"
#include <src/fend/uidelegates/bucketdelegate.h>
#include <src/fend/uidelegates/tableitemdelegate.h>
#include "src/config/apis.h"
#include <QFileDialog>
#include <QJsonObject>
#include <QMessageBox>
#include <src/helper/filehelper.h>
#include <QStyleOption>
#include <QPainter>
#include <src/fend/uicommond/uimessagebox.h>

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
    // 设置鼠标点击排序
    ui->tableViewObjects->setSortingEnabled(true);

    // 初始化翻页按钮
    ui->widgetPageFlip->setRowPerPageItems({5, 10, 20});

    // 设置样式
    ui->pushButtonBucketList->setProperty("style_button", "main");
    ui->tableViewObjects->setStyleSheet(R"(
        QTableView {
    border: none;
    border-top: 1px solid #dddddd;
    }
    QTableView::item {
    height: 30px;
    }
    QTableView::item {
    border-bottom: 1px solid #dddddd;
    }
    QTableView::item:hover,
                       QTableView::item:selected {
        background-color: rgba(221, 221, 221, 0.4); /* 等价于 #66dddddd */
    color: blue;
    })");

    // 信号槽连接
    connect(MANAGER_GLOBAL->managerSignals, &ManagerSignals::objectsSuccess, this, &ObjectsWidget::onObjectsSuccess);
    connect(ui->widgetBreadCrumb, &BreadCrumdWidget::pathChanged, this, &ObjectsWidget::onPathChanged);
    connect(ui->widgetPageFlip, &PageFlipWidget::pageNumberChanged,this, &ObjectsWidget::onPageNumberChanged);
    connect(MANAGER_GLOBAL->managerSignals, &ManagerSignals::uploadSuccess, this, &ObjectsWidget::onUploadSuccess);
    connect(MANAGER_GLOBAL->managerSignals, &ManagerSignals::downloadSuccess, this, &ObjectsWidget::onDownloadSuccess);

    // 设置itemdelegate
    ui->tableViewObjects->setItemDelegate(new TableItemDelegate(ui->tableViewObjects));
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
    // 设置行高
    QStandardItemModel* model = MANAGER_GLOBAL->managerModels->modelObjects();
    for (int i = 0; i < model->rowCount(); ++i) {
        ui->tableViewObjects->setRowHeight(i, 40);
    }
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


void ObjectsWidget::on_pushButtonUpload_clicked()
{
    static QString lastDir = "./";
    // 获取本地文件
    QString filePath = QFileDialog::getOpenFileName(this, "上传文件", lastDir);
    QFileInfo fileInfo(filePath);
    filePath = filePath.replace("\\", "/");
    if (fileInfo.isFile() && fileInfo.exists()) {
        QUuid uuid = QUuid::createUuid();
        QJsonObject params;
        QString jobId = uuid.toString();
        QString key = MANAGER_GLOBAL->managerCloud->currentDir() + fileInfo.fileName();
        params["jobId"] = jobId;
        params["bucketName"] = MANAGER_GLOBAL->managerCloud->currentBucketName();
        params["key"] = key;
        params["localPath"] = filePath;
        MANAGER_GLOBAL->gateway->send(API::OBJECTS::PUT, params);
        lastDir = fileInfo.dir().absolutePath();  // 记忆上次打开的目录
        emit MANAGER_GLOBAL->managerSignals->startUpload(jobId, key ,filePath);
    }
    qDebug() << "upload file: " << filePath;
}


void ObjectsWidget::on_pushButtonDownload_clicked()
{
    QModelIndex index = ui->tableViewObjects->currentIndex();
    if (!index.isValid()) { // inValid的情况：没有选中任何行
        UiMessageBox().showMessage("下载", "请选择要下载的文件", {"确定"},180,60);
        return;
    }
    MyObject object = index.data(Qt::UserRole).value<MyObject>(); // 获取当前选中的对象
    if (object.isDir()) { // 仅当选中的是文件时响应
        UiMessageBox().showMessage("下载", "只能选择文件进行下载", {"确定"},180,60);
        return;
    }
    QString fileName = index.data().toString();
    static QString lastDir = "./";
    QString savePath = QFileDialog::getSaveFileName(this, "保存文件", FileHelper::joinPath(lastDir, fileName));
    // 用户取消了保存
    if (savePath.isEmpty()) { // 当用户点击 取消 时，它会返回一个 空字符串 ("")。
        return;
    }
    QFileInfo fileInfo(lastDir);
    savePath = savePath.replace("\\", "/");
    QJsonObject params;
    QString jobId = QUuid::createUuid().toString();
    QString key = MANAGER_GLOBAL->managerCloud->currentDir() + fileName;
    params["jobId"] = jobId;
    params["bucketName"] = MANAGER_GLOBAL->managerCloud->currentBucketName();
    params["key"] = key;
    params["localPath"] = savePath;
    MANAGER_GLOBAL->gateway->send(API::OBJECTS::GET, params);
    lastDir = fileInfo.dir().absolutePath();
    emit MANAGER_GLOBAL->managerSignals->startDownload(jobId, key, savePath,object.size);
/*
QFileInfo fileInfo(path) 取的是 本地磁盘上已有文件的大小。
但是 下载前本地文件还没生成（或者是 0 大小的空文件），所以 fileInfo.size() 返回的是 0。
于是进度条 setRange(0,0) → 进入不确定模式（Busy Mode，进度条滚动）。必须用object.size

上传时，本地文件已经存在，所以 QFileInfo(localPath).size() 可以正确返回文件大小。
下载时，本地文件还没写入，所以 QFileInfo(localPath).size() 只能得到 0，必须用 object.size。
*/
}

void ObjectsWidget::onUploadSuccess(const QString &jobId)
{
    Q_UNUSED(jobId);
    on_pushButtonRefresh_clicked(); // 刷新当前路径
    showMessageBox("上传成功", "上传文件成功");
}

void ObjectsWidget::onDownloadSuccess(const QString &jobId)
{
    Q_UNUSED(jobId);
    showMessageBox("下载成功", "下载文件成功");
}

void ObjectsWidget::showMessageBox(const QString &title, const QString &messsage)
{
    QMessageBox box(QMessageBox::Information, title, messsage, QMessageBox::NoButton, this);
    box.addButton("确定", QMessageBox::AcceptRole);
    box.exec();
}

