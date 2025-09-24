#include "managermodels.h"
#include "src/middle/managerglobal.h"

#include <src/middle/models/mybucket.h>

#include <src/middle/signals/managersignals.h>

ManagerModels::ManagerModels(QObject *parent)
    : QObject{parent},
      m_modelBuckets(new QStandardItemModel(this))
      , m_modelObjects(new QStandardItemModel(this))
{
    // 连接信号槽，当获取到存储桶列表时更新模型
    connect(MANAGER_GLOBAL->managerSignals, &ManagerSignals::bucketsSuccess,
            this, &ManagerModels::setBuckets);
    // 连接信号槽，当获取到对象列表时更新模型
    connect(MANAGER_GLOBAL->managerSignals, &ManagerSignals::objectsSuccess,
            this, &ManagerModels::setObjects);
    initBucketsTable();
    initObjectsTable();
}

QStandardItemModel *ManagerModels::modelBuckets() const
{
    return m_modelBuckets;
}

QStandardItemModel *ManagerModels::modelObjects() const
{
    return m_modelObjects;
}

void ManagerModels::setBuckets(const QList<MyBucket> &buckets)
{
    m_modelBuckets->setRowCount(buckets.size()); // 行
    for (int i = 0; i < buckets.size(); ++i) {
        const MyBucket& bucket = buckets.at(i);
        // 第一列
        QModelIndex index_name = m_modelBuckets->index(i, 0);
        m_modelBuckets->setData(index_name, bucket.name);
        m_modelBuckets->setData(index_name, QString("存储桶名称: %1").arg(bucket.name), Qt::ToolTipRole);
        // 第二列
        QModelIndex index_location = m_modelBuckets->index(i,1);
        m_modelBuckets->setData(index_location, bucket.location);
        // 第三列
        QModelIndex index_create_date = m_modelBuckets->index(i,2);
        m_modelBuckets->setData(index_create_date, bucket.createDate);
    }
    // 默认按照时间排序，降序，以便最新创建的存储桶显示在最上面
    m_modelBuckets->sort(2, Qt::DescendingOrder);
}

void ManagerModels::setObjects(const QList<MyObject> &objects)
{
    m_modelObjects->setRowCount(objects.size()); // 行
    for (int i = 0; i < objects.size(); ++i) {
        const MyObject& object = objects.at(i);
        // 名称
        QModelIndex index_name = m_modelObjects->index(i, 0);
        m_modelObjects->setData(index_name, object.name);
        QVariant var;
        var.setValue(object);
        m_modelObjects->setData(index_name, var, Qt::UserRole); // 存储对象信息
        // 大小
        QModelIndex index_size = m_modelObjects->index(i, 1);
        m_modelObjects->setData(index_size, object.size);
        // 修改时间
        QModelIndex index_last_modified = m_modelObjects->index(i, 2);
        m_modelObjects->setData(index_last_modified, object.lastModified);
    }
}

void ManagerModels::initBucketsTable()
{
    // 设置标题内容
    QStringList headers = {"存储桶名称", "存储桶地域", "创建时间"};
    m_modelBuckets->setColumnCount(headers.size());
    m_modelBuckets->setHorizontalHeaderLabels(headers);
}

void ManagerModels::initObjectsTable()
{
    // 设置标题内容
    QStringList headers = {"对象名称", "大小", "修改时间"};
    m_modelObjects->setColumnCount(headers.size());
    m_modelObjects->setHorizontalHeaderLabels(headers);
}
