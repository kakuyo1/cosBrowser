#include "breadcrumdwidget.h"
#include "ui_breadcrumdwidget.h"

BreadCrumdWidget::BreadCrumdWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::BreadCrumdWidget)
    , m_rootName("")
    , m_model(new QStandardItemModel())
{
    ui->setupUi(this);
    // 设置视图模型
    ui->listView->setModel(m_model);
    ui->listView->setFlow(QListView::LeftToRight); // 水平布局
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers); // 禁止编辑
    ui->listView->setCursor(Qt::PointingHandCursor); // 设置鼠标形状

    // 关联点击信号与槽
    connect(ui->listView, &QListView::clicked, this, &BreadCrumdWidget::onItemClicked);

    // 设置样式
    setFixedHeight(30);

    // 默认路径为空
    setPath("");

    setCursor(Qt::PointingHandCursor);

    // 设置样式
    ui->listView->setStyleSheet(R"(
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
}

BreadCrumdWidget::~BreadCrumdWidget()
{
    delete ui;
}

void BreadCrumdWidget::setPath(const QString &path)
{
    if (path.isEmpty()) {
        return;
    }
    QStringList pathList = path.split("/", Qt::SkipEmptyParts);
    setPath(pathList);
}

void BreadCrumdWidget::setPath(const QStringList &pathList)
{
    if (pathList.isEmpty()) {
        return;
    }
    clearPath(); // 清空原本路径
    m_rootName = pathList.first(); // 设置根目录名称
    for (const QString& name : pathList) {
        addPathItem(name); // 添加路径元素
    }
}

void BreadCrumdWidget::clearPath()
{
    m_model->clear();
    ui->listView->setModel(m_model);
    m_rootName = "";
}

QString BreadCrumdWidget::currentPath() const
{
    return getFullPathByItem();
}

QString BreadCrumdWidget::rootName() const
{
    return m_rootName;
}

bool BreadCrumdWidget::isEmptyPath() const
{
    return ui->listView->model()->rowCount() == 0;
}

bool BreadCrumdWidget::isRootPath() const
{
    // 如果当前路径等于根目录名称，则表示在根目录
    return getFullPathByItem() == m_rootName;
}

void BreadCrumdWidget::addPathItem(const QString &name)
{
    if (name.isEmpty()) {
        return;
    }
    QStandardItem* item = new QStandardItem;
    item->setText(name);
    // 如果不是第一个元素，添加箭头图标
    if (!isEmptyPath()) {
        item->setIcon(QIcon(":/static/image/right.png"));
    }
    m_model->appendRow(item); // 添加到模型
}

void BreadCrumdWidget::onItemClicked(const QModelIndex &index)
{
    QStandardItem* new_item = m_model->itemFromIndex(index);
    QStandardItem* old_item = currentPathItem();

    if (new_item == old_item) {
        // 点击了当前路径元素，刷新
        QString fullPath = getFullPathByItem(new_item);
        emit refresh(fullPath);
    } else if (new_item != old_item) {
        // 点击了非当前路径元素，更新路径
        QString fullPath = getFullPathByItem(new_item);
        emit pathChanged(fullPath);
    }
}

QStandardItem *BreadCrumdWidget::currentPathItem() const
{
    // 即最后一个元素
    return m_model->item(m_model->rowCount() - 1);
}

QString BreadCrumdWidget::getFullPathByItem(QStandardItem *item) const
{
    // 如果没有传入元素，则使用当前路径元素(每次点击都会重新刷新，而不是把点击处后面的路径元素进行删除)
    if (!item) {
        item = currentPathItem();
    }
    int index = item->row();
    QStringList pathList;
    for (int i = 0; i <= index; ++i) {
        pathList.append(m_model->item(i)->text());
    }
    return pathList.join("/");
}
